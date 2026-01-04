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
#include "PachetScreen.h"

// INCLUDE-URI TEMA 3
#include "Gestiune.h"
#include "Utilitare.h"
#include "HistoryScreen.h"
#include "UsernameScreen.h"

bool loadFont(sf::Font& fontRef, const std::string& filename) {
    if (!fontRef.openFromFile(filename)) {
        std::cerr << "Eroare CRITICA: Nu s-a putut incarca fontul '" << filename << "'!\n";
        return false;
    }
    return true;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    // --- INSTANȚIERI ȘABLOANĂ TEMA 3 ---
    Gestiune<std::string> istoricMeciuri("Istoric Rezultate");
    Gestiune<jucatorptr> istoricAchizitii("Jucatori Contractati");

    manageri::getInstance().citire_baza_managerGUI();
    manageri::getInstance().citire_toti_jucatorii_si_echipe();

    sf::Font globalFont;
    if (!loadFont(globalFont, "ARIAL.ttf")) return -1;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1000, 700)), "Volei Manager PO", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    BazaDeDateptr baza = manageri::getInstance().getBazaDeDate();
    Echipeptr echipaMea = manageri::getInstance().getEchipaManager();

    if (!echipaMea) {
        std::cerr << "EROARE: Echipa managerului nu a fost creata!" << std::endl;
        return -1;
    }

    std::vector<Screenptr> screens(14);

    std::shared_ptr<Liga> ligaProgres = std::make_shared<Liga>(baza);
    ligaProgres->creare_liga();
    ligaProgres->adaugare_echipa(echipaMea);

    Echipeptr echipaAdversaProvizorie = std::make_shared<Echipe>("Adversar Rezerva");
    echipaAdversaProvizorie->set_overall(80);

    screens[SCREEN_MAIN_MENU] = std::make_shared<MainMenuScreen>(globalFont);
    screens[SCREEN_TRANSFER] = std::make_shared<TransferScreen>(baza, echipaMea, globalFont);
    screens[SCREEN_DRAFT] = std::make_shared<InitialDraftScreen>(echipaMea, baza, globalFont);
    screens[SCREEN_FIRST_SIX] = std::make_shared<FirstSixScreen>(echipaMea, globalFont);
    screens[SCREEN_TIMEOUT] = std::make_shared<TimeoutScreen>(echipaMea, baza, globalFont);
    screens[MATCH_SCREEN] = std::make_shared<MatchScreen>(echipaMea, echipaAdversaProvizorie, globalFont);
    screens[SCREEN_MATCH_LEAGUE] =  std::make_shared<MatchLeague>(echipaMea, echipaAdversaProvizorie, globalFont);
    screens[SCREEN_LIGA] = std::make_shared<LigaScreen>(ligaProgres, echipaMea, globalFont);
    screens[SCREEN_SCOREBOARD] = std::make_shared<ScoreboardScreen>(ligaProgres, globalFont);

    screens[10] = std::make_shared<HistoryScreen<std::string>>(istoricMeciuri, globalFont, "ISTORIC MECIURI");
    screens[11] = std::make_shared<HistoryScreen<jucatorptr>>(istoricAchizitii, globalFont, "ISTORIC ACHIZITII");
    screens[12] = std::make_shared<UsernameScreen>(globalFont);
    screens[13] = std::make_shared<PackScreen>(globalFont, echipaMea, baza->getLista());


    int current_screen = 12;

    while (current_screen != SCREEN_EXIT && window.isOpen()) {
        int next_screen_id = SCREEN_EXIT;

        if (auto* f6 = dynamic_cast<FirstSixScreen*>(screens[SCREEN_FIRST_SIX].get())) {
            f6->update_jucatori(echipaMea->get_jucatori());
        }

        if (current_screen >= 0 && current_screen < static_cast<int>(screens.size()) && screens[current_screen] != nullptr) {
            int prev_screen = current_screen;
            next_screen_id = screens[current_screen]->run(window);

            // 1. REPARARE LOGICĂ MECI AMICAL (MATCH_SCREEN)
            if (next_screen_id == MATCH_SCREEN) {
                if (auto msPtr = std::dynamic_pointer_cast<MatchScreen>(screens[MATCH_SCREEN])) {
                    if (prev_screen != SCREEN_TIMEOUT) {
                        // AICI: Alegem o echipă reală din baza de date, nu placeholder
                        Echipeptr advRandom = baza->alege_echipa_random();
                        msPtr->setAdversar(advRandom ? advRandom : echipaAdversaProvizorie);
                        msPtr->resetare_scoruri();
                    }
                }
            }

            // 2. REPARARE LOGICĂ MECI LIGĂ (SCREEN_MATCH_LEAGUE)
            else if (next_screen_id == SCREEN_MATCH_LEAGUE) {
                if (auto matchLeaguePtr = std::dynamic_pointer_cast<MatchLeague>(screens[SCREEN_MATCH_LEAGUE])) {
                    if (prev_screen == SCREEN_LIGA) {
                        // AICI: Extragem adversarul corect din calendarul ligii
                        Echipeptr adv = ligaProgres->getNextAdversarPentruManager();
                        if (adv) {
                            matchLeaguePtr->setAdversar(adv);
                            matchLeaguePtr->resetare_scoruri();
                        } else {
                            next_screen_id = SCREEN_LIGA; // Niciun meci rămas
                        }
                    }
                }
            }

            // 3. ÎNREGISTRARE REZULTATE MECI LIGĂ ÎN ISTORIC
            else if (prev_screen == SCREEN_MATCH_LEAGUE && next_screen_id == SCREEN_LIGA) {
                if (auto mlPtr = std::dynamic_pointer_cast<MatchLeague>(screens[SCREEN_MATCH_LEAGUE])) {
                    Echipeptr adversar = mlPtr->getEchipaAdversa();
                    Echipeptr castigator = mlPtr->getMatchWinner();

                    if (adversar && castigator) {
                        // Salvare în Gestiune<std::string>
                        std::string res = echipaMea->getNume() + " vs " + adversar->getNume() + " | Final: " + castigator->getNume();
                        istoricMeciuri.adauga(res);

                        ligaProgres->registerMatchResult(adversar, castigator);
                        ligaProgres->meciuri();

                        if (ligaProgres->isSeasonFinished()) {
                            ligaProgres->finalizeSeason(echipaMea);
                            next_screen_id = SCREEN_MAIN_MENU;
                        }

                        if (auto lsPtr = std::dynamic_pointer_cast<LigaScreen>(screens[SCREEN_LIGA])) {
                            lsPtr->notifyMatchFinished();
                        }
                    }
                }
            }

            // LOGICĂ TIMEOUT (PENTRU REVENIRE CORECTĂ)
            if (next_screen_id == SCREEN_TIMEOUT) {
                if (auto timeoutPtr = std::dynamic_pointer_cast<TimeoutScreen>(screens[SCREEN_TIMEOUT])) {
                    timeoutPtr->setReturnScreenId(prev_screen);
                }
            }
        }
        current_screen = next_screen_id;
    }
    return 0;
}