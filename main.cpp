#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
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
#include "Gestiune.h"
#include "Utilitare.h"
#include "HistoryScreen.h"
#include "UsernameScreen.h"
#include "PachetScreen.h"
#include "Strategies.h"

using namespace std;

bool loadFont(sf::Font& fontRef, const std::string& filename) {
    return fontRef.openFromFile(filename);
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Gestiune<std::string> istoricMeciuri("Istoric Rezultate");
    Gestiune<jucatorptr> istoricAchizitii("Jucatori Contractati");

    cout << istoricMeciuri.getNrElemente()
         << gasesteMaxim<int>(10, 20)
         << constringeValoare<int>(150, 0, 100) << endl;

    manageri::getInstance().citire_baza_managerGUI();
    manageri::getInstance().citire_toti_jucatorii_si_echipe();

    sf::Font globalFont;
    if (!loadFont(globalFont, "ARIAL.ttf")) return -1;

    BazaDeDateptr baza = manageri::getInstance().getBazaDeDate();
    Echipeptr echipaMea = manageri::getInstance().getEchipaManager();
    if (!echipaMea) return -1;

    if (!baza->getLista().empty()) {
        istoricAchizitii.adauga(baza->getLista()[0]);
    }

    shared_ptr<Liga> ligaProgres = make_shared<Liga>(baza);

    // --- MODIFICAREA CRITICA PENTRU A NU JUCA CONTRA TA ---
    ligaProgres->creare_liga();            // 1. Intai bagam robotii
    ligaProgres->adaugare_echipa(echipaMea); // 2. Tu esti ultimul (index n-1)
    // -----------------------------------------------------

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1000, 700)), "Volei Manager PO", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    vector<Screenptr> screens(14);
    Echipeptr advRezerva = make_shared<Echipe>("Adversar Rezerva");
    advRezerva->set_overall(80);

    screens[SCREEN_MAIN_MENU] = make_shared<MainMenuScreen>(globalFont);
    screens[SCREEN_TRANSFER] = make_shared<TransferScreen>(baza, echipaMea, globalFont);
    screens[SCREEN_DRAFT] = make_shared<InitialDraftScreen>(echipaMea, baza, globalFont);
    screens[SCREEN_FIRST_SIX] = make_shared<FirstSixScreen>(echipaMea, globalFont);
    screens[SCREEN_TIMEOUT] = make_shared<TimeoutScreen>(echipaMea, baza, globalFont);
    screens[MATCH_SCREEN] = make_shared<MatchScreen>(echipaMea, advRezerva, globalFont);
    screens[SCREEN_MATCH_LEAGUE] = make_shared<MatchLeague>(echipaMea, advRezerva, globalFont, istoricMeciuri);
    screens[SCREEN_LIGA] = make_shared<LigaScreen>(ligaProgres, echipaMea, globalFont);
    screens[SCREEN_SCOREBOARD] = make_shared<ScoreboardScreen>(ligaProgres, globalFont);
    screens[10] = make_shared<HistoryScreen<string>>(istoricMeciuri, globalFont, "ISTORIC MECIURI");
    screens[11] = make_shared<HistoryScreen<jucatorptr>>(istoricAchizitii, globalFont, "ISTORIC ACHIZITII");
    screens[12] = make_shared<UsernameScreen>(globalFont);
    screens[13] = make_shared<PackScreen>(globalFont, echipaMea, baza->getLista(), istoricAchizitii);

    int current_screen = 12;

    while (current_screen != SCREEN_EXIT && window.isOpen()) {
        int next_screen_id = SCREEN_EXIT;

        if (current_screen >= 0 && current_screen < static_cast<int>(screens.size()) && screens[current_screen]) {
            int prev_screen = current_screen;
            next_screen_id = screens[current_screen]->run(window);

            if (next_screen_id == SCREEN_FIRST_SIX) {
                if (auto f6 = dynamic_pointer_cast<FirstSixScreen>(screens[SCREEN_FIRST_SIX]))
                    f6->update_jucatori(echipaMea->get_jucatori());
            }

            if (next_screen_id == MATCH_SCREEN) {
                if (prev_screen != SCREEN_TIMEOUT) {
                    if (auto msPtr = dynamic_pointer_cast<MatchScreen>(screens[MATCH_SCREEN])) {
                        Echipeptr advRandom = baza->alege_echipa_random();
                        msPtr->setAdversar(advRandom ? advRandom : advRezerva);
                        msPtr->resetare_scoruri();
                    }
                }
            }

            if (next_screen_id == SCREEN_MATCH_LEAGUE) {
                if (prev_screen != SCREEN_TIMEOUT) {
                    if (auto mlPtr = dynamic_pointer_cast<MatchLeague>(screens[SCREEN_MATCH_LEAGUE])) {
                        Echipeptr adv = ligaProgres->getNextAdversarPentruManager();

                        // Protectie: Daca nu gaseste adversar, reseteaza etapa si cauta iar
                        if (!adv) {
                            ligaProgres->reset_etapa();
                            adv = ligaProgres->getNextAdversarPentruManager();
                        }

                        // Protectie suplimentara: nu juca contra ta
                        if (adv == echipaMea) {
                            ligaProgres->reset_etapa();
                            adv = ligaProgres->getNextAdversarPentruManager();
                        }

                        mlPtr->setAdversar(adv ? adv : advRezerva);
                    }
                }
            }

            if (prev_screen == SCREEN_MATCH_LEAGUE && next_screen_id == SCREEN_LIGA) {
                if (auto mlPtr = dynamic_pointer_cast<MatchLeague>(screens[SCREEN_MATCH_LEAGUE])) {
                    Echipeptr adv = mlPtr->getAdversar();
                    Echipeptr win = mlPtr->getMatchWinner();

                    if (adv && win && adv != advRezerva && adv != echipaMea) {
                        ligaProgres->registerMatchResult(adv, win);

                        string log = "Liga: " + echipaMea->getNume() + " vs " + adv->getNume();
                        istoricMeciuri.adauga(log);

                        ligaProgres->meciuri();
                    }
                }

                if (auto ls = dynamic_pointer_cast<LigaScreen>(screens[SCREEN_LIGA])) {
                    ls->notifyMatchFinished();
                }
            }

            if (next_screen_id == SCREEN_TIMEOUT) {
                if (auto timeoutPtr = dynamic_pointer_cast<TimeoutScreen>(screens[SCREEN_TIMEOUT]))
                    timeoutPtr->setReturnScreenId(prev_screen);
            }
        }
        current_screen = next_screen_id;
    }
    return 0;
}