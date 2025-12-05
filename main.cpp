#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <stdexcept>
#include <array>

#include "manageri.h"
#include "Screen.h"
#include "MainMenuScreen.h"
#include "TransferScreen.h"
#include "InitialDraftScreen.h"
#include "MatchScreen.h"
#include "TimeoutScreen.h"
#include "FirstSixScreen.h"
#include "Echipe.h"
#include "Liga.h"
#include "LigaScreen.h"
#include "ScoreboardScreen.h"
#include "MatchLeague.h"



bool loadFont(sf::Font& fontRef, const std::string& filename) {
    if (!fontRef.openFromFile(filename)) {
        std::cerr << "Eroare CRITICA: Nu s-a putut incarca fontul '" << filename << "'!\n";
        return false;
    }
    return true;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    manageri manager;
    manager.citire_baza_managerGUI();
    manager.citire_toti_jucatorii_si_echipe();

    sf::Font globalFont;
    if (!loadFont(globalFont, "ARIAL.ttf")) {
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1000, 700)),
                            "Volei Manager PO",
                            sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    BazaDeDateptr baza = manager.getBazaDeDate();
    Echipeptr echipaMea = manager.getEchipaManager();

    std::vector<Screenptr> screens(10);

    std::shared_ptr<Liga> ligaProgres = std::make_shared<Liga>(baza);

    ligaProgres->creare_liga();
    ligaProgres->adaugare_echipa(echipaMea);

    Echipeptr echipaAdversaProvizorie = std::make_shared<Echipe>("Adversar Placeholder");
    echipaAdversaProvizorie->set_overall(80);

    screens[SCREEN_MAIN_MENU] = std::make_shared<MainMenuScreen>(globalFont);
    screens[SCREEN_TRANSFER] = std::make_shared<TransferScreen>(baza, echipaMea, globalFont);
    screens[SCREEN_DRAFT] = std::make_shared<InitialDraftScreen>(echipaMea, baza, globalFont);
    screens[SCREEN_FIRST_SIX] = std::make_shared<FirstSixScreen>(echipaMea, globalFont);

    screens[MATCH_SCREEN] = std::make_shared<MatchScreen>(echipaMea, echipaAdversaProvizorie, globalFont);
    screens[SCREEN_MATCH_LEAGUE] =  std::make_shared<MatchLeague>(echipaMea, echipaAdversaProvizorie, globalFont);

    screens[SCREEN_LIGA] = std::make_shared<LigaScreen>(ligaProgres, echipaMea, baza, globalFont, window);
    screens[SCREEN_SCOREBOARD] = std::make_shared<ScoreboardScreen>(ligaProgres, globalFont);

    screens[SCREEN_TIMEOUT] = std::make_shared<TimeoutScreen>(echipaMea, baza, globalFont);


    int current_screen = SCREEN_DRAFT;

    while (current_screen != SCREEN_EXIT && window.isOpen()) {

        int next_screen_id = SCREEN_EXIT;

        Screen* firstSixScreenPtrRaw = screens[SCREEN_FIRST_SIX].get();
        if (FirstSixScreen* firstSixScreenPtr = dynamic_cast<FirstSixScreen*>(firstSixScreenPtrRaw)) {
            firstSixScreenPtr->update_jucatori(echipaMea->get_jucatori());
        }

        if (current_screen >= 0 && current_screen < screens.size() && screens[current_screen] != nullptr) {

            int prev_screen = current_screen;
            next_screen_id = screens[current_screen]->run(window);

            if (next_screen_id == SCREEN_TIMEOUT) {
                std::shared_ptr<TimeoutScreen> timeoutPtr = std::dynamic_pointer_cast<TimeoutScreen>(screens[SCREEN_TIMEOUT]);

                if (timeoutPtr) {
                    timeoutPtr->setReturnScreenId(prev_screen);
                }
            }


            if (next_screen_id == SCREEN_MATCH_LEAGUE) {

                std::shared_ptr<MatchLeague> matchLeaguePtr = std::dynamic_pointer_cast<MatchLeague>(screens[SCREEN_MATCH_LEAGUE]);

                if (!matchLeaguePtr) {
                     next_screen_id = SCREEN_LIGA;
                }

                else if (prev_screen == SCREEN_LIGA) {

                    Echipeptr echipaAdversaNoua = ligaProgres->getNextAdversarPentruManager();

                    if (echipaAdversaNoua) {
                        matchLeaguePtr->setAdversar(echipaAdversaNoua);
                        matchLeaguePtr->resetare_scoruri();
                    } else {
                        next_screen_id = SCREEN_LIGA;
                    }
                }
            }

            else if (prev_screen == SCREEN_MATCH_LEAGUE && next_screen_id == SCREEN_LIGA) {

                std::shared_ptr<MatchLeague> matchLeaguePtr =
                    std::dynamic_pointer_cast<MatchLeague>(screens[SCREEN_MATCH_LEAGUE]);

                if (matchLeaguePtr) {
                    Echipeptr adversar = matchLeaguePtr->getEchipaAdversa();
                    Echipeptr castigator = matchLeaguePtr->getMatchWinner();

                    if (adversar && castigator) {
                        ligaProgres->registerMatchResult(adversar, castigator);

                        ligaProgres->meciuri();

                        if (ligaProgres->isSeasonFinished()) {
                            ligaProgres->finalizeSeason(echipaMea);
                            next_screen_id = SCREEN_MAIN_MENU;
                        }

                        std::shared_ptr<LigaScreen> ligaScreenPtr =
                            std::dynamic_pointer_cast<LigaScreen>(screens[SCREEN_LIGA]);
                        if (ligaScreenPtr) {
                            ligaScreenPtr->notifyMatchFinished();
                        }
                    }
                }
            }

            else if (next_screen_id == MATCH_SCREEN) {
                std::shared_ptr<MatchScreen> matchScreenPtr = std::dynamic_pointer_cast<MatchScreen>(screens[MATCH_SCREEN]);
                if (matchScreenPtr) {
                    if (prev_screen != SCREEN_TIMEOUT) {
                        Echipeptr echipaAdversaNoua = baza->alege_echipa_random();
                        matchScreenPtr->setAdversar(echipaAdversaNoua ? echipaAdversaNoua : echipaAdversaProvizorie);
                        matchScreenPtr->resetare_scoruri();
                    }
                }
            }


        } else {
            std::cerr << "Eroare CRITICA in main loop: Stare ecran invalida (" << current_screen << ") sau pointer NULL!" << std::endl;
            if (current_screen != SCREEN_MAIN_MENU && screens[SCREEN_MAIN_MENU] != nullptr) {
                next_screen_id = SCREEN_MAIN_MENU;
            } else {
                 next_screen_id = SCREEN_EXIT;
            }
        }

        current_screen = next_screen_id;
    }

    return 0;
}
