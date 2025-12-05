#include "MatchLeague.h"

#include "MatchScreen.h"
#include "TableScore.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

namespace {
    struct SeedRandom {
        SeedRandom() { std::srand(static_cast<unsigned int>(std::time(nullptr))); }
    } seed;
}


const int PUNCTE_NECESARE_SET = 25;
const int SETURI_NECESARE_MECI = 3;
const float RITM_SIMULARE_SEC = 0.5f;
MatchLeague::MatchLeague(Echipeptr myTeam, Echipeptr opponent, sf::Font& font)
    : echipaMea(myTeam), echipaAdversa(opponent), 
      tablaScor(font, myTeam, opponent, 30.0f, 50.0f),
      statusText(font),
      timeoutButton(50.0f, 600.0f, 200.0f, 50.0f, "Cere Timeout", font),
      continueButton(50.0f, 600.0f, 200.0f, 50.0f, "Continua (Meniu)", font)
{
    echipaMea->resetPunctaj();
    echipaMea->resetSetur();
    echipaAdversa->resetPunctaj();
    echipaAdversa->resetSetur();

    echipaMea->serve_on();
    echipaAdversa->serve_off();

    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::Cyan);
    statusText.setPosition(sf::Vector2f(50.0f, 150.0f));

    continueButton.setBaseColor(sf::Color(50, 150, 50));
    continueButton.setString("Continua (Meniu)");
}

void MatchLeague::toggleServe() {
    isMyTeamServing = !isMyTeamServing;
    if (isMyTeamServing) {
        echipaMea->serve_on();
        echipaAdversa->serve_off();
    } else {
        echipaMea->serve_off();
        echipaAdversa->serve_on();
    }
    tablaScor.setServingTeamHighlight(isMyTeamServing);
}



void MatchLeague::setAdversar(Echipeptr newOpponent) {
    if (newOpponent) {
        echipaAdversa = newOpponent;
        tablaScor.setOpponent(newOpponent);

        echipaMea->resetPunctaj();
        echipaMea->resetSetur();
        echipaAdversa->resetPunctaj();
        echipaAdversa->resetSetur();

        isMyTeamServing = true;
        echipaMea->serve_on();
        echipaAdversa->serve_off();
        tablaScor.setServingTeamHighlight(true);

        currentState = SIMULATION_IN_PROGRESS;
        next_screen_id = SCREEN_MATCH_LEAGUE;
    }
}


Echipeptr MatchLeague::getEchipaAdversa() const {
    return echipaAdversa;
}

Echipeptr MatchLeague::getMatchWinner() const {
    if (echipaMea->getSeturi() > echipaAdversa->getSeturi()) {
        return echipaMea;
    } else if (echipaAdversa->getSeturi() > echipaMea->getSeturi()) {
        return echipaAdversa;
    }
    return nullptr;
}

void MatchLeague::resetForNewSet() {
    echipaMea->resetPunctaj();
    echipaAdversa->resetPunctaj();

    toggleServe();
}

void MatchLeague::checkSetEnd() {
    int pMea = echipaMea->getPunctaj();
    int pAdv = echipaAdversa->getPunctaj();

    if (pMea >= PUNCTE_NECESARE_SET && pMea >= pAdv + 2) {

        echipaMea->modifSetur();

        echipaMea->updateMomentum(Echipe::MOMENTUM_SET_WIN);
        echipaAdversa->updateMomentum(Echipe::MOMENTUM_SET_LOSS);

        statusText.setString("SET CASTIGAT de " + echipaMea->getNume() + "!");

        if (echipaMea->getSeturi() == SETURI_NECESARE_MECI) {
            finalizeMatch();
        } else {
            resetForNewSet();
        }

    }

    else if (pAdv >= PUNCTE_NECESARE_SET && pAdv >= pMea + 2) {

        echipaAdversa->modifSetur();

        echipaAdversa->updateMomentum(Echipe::MOMENTUM_SET_WIN);
        echipaMea->updateMomentum(Echipe::MOMENTUM_SET_LOSS);

        statusText.setString("SET PIERDUT in fata " + echipaAdversa->getNume() + ".");

        if (echipaAdversa->getSeturi() == SETURI_NECESARE_MECI) {
            finalizeMatch();
        } else {
            resetForNewSet();
        }
    }
}

float MatchLeague::getWeightedTeamScore(Echipeptr echipa) const {
    if (!echipa) return 0.0f;

    float impact_total = 0.0f;
    const auto& titulari = echipa->get_jucatori_pe_teren();

    if (titulari.size() != 6) {
        return (float)echipa->get_overall();
    }

    for (const auto& titular : titulari) {
        impact_total += titular->obtine_scor_tactica_final();
    }

    return impact_total / 6.0f;
}

void MatchLeague::simulatePoint() {
    if (actionClock.getElapsedTime().asSeconds() < RITM_SIMULARE_SEC) {
        return;
    }
    actionClock.restart();

    float scorMea = getWeightedTeamScore(echipaMea);
    float scorAdversa = getWeightedTeamScore(echipaAdversa);

    scorMea *= (echipaMea->getMomentum() / 0.80f);
    scorAdversa *= (echipaAdversa->getMomentum() / 0.80f);

    if (echipaMea->get_serve()) scorMea *= 1.15f;
    else scorAdversa *= 1.15f;

    float putere = 1.8f;
    float sansa_mea = std::pow(scorMea, putere) /
                     (std::pow(scorMea, putere) + std::pow(scorAdversa, putere));

    float r = (float)std::rand() / RAND_MAX;

    if (r < sansa_mea) {
        echipaMea->modifPunctaj();
        echipaMea->updateMomentum(Echipe::MOMENTUM_POINT_WIN);
        echipaAdversa->updateMomentum(Echipe::MOMENTUM_POINT_LOSS);
        if (!echipaMea->get_serve()) toggleServe();
    } else {
        echipaAdversa->modifPunctaj();
        echipaAdversa->updateMomentum(Echipe::MOMENTUM_POINT_WIN);
        echipaMea->updateMomentum(Echipe::MOMENTUM_POINT_LOSS);
        if (echipaMea->get_serve()) toggleServe();
    }

    checkSetEnd();
}


void MatchLeague::finalizeMatch() {
    currentState = MATCH_END_DELAY;

    actionClock.restart();


    if (echipaMea->getSeturi() > echipaAdversa->getSeturi()) {
        finalMessage = "VICTORIE! Ai castigat meciul cu scorul " + std::to_string(echipaMea->getSeturi()) + "-" + std::to_string(echipaAdversa->getSeturi()) + ".";
        statusText.setFillColor(sf::Color::Green);
    } else {
        finalMessage = "INFRANGERE! Ai pierdut meciul cu scorul " + std::to_string(echipaMea->getSeturi()) + "-" + std::to_string(echipaAdversa->getSeturi()) + ".";
        statusText.setFillColor(sf::Color::Red);
    }

    continueButton.setPosition(timeoutButton.getPosition());
}


void MatchLeague::update() {
    tablaScor.updateScore();

    if (currentState == SIMULATION_IN_PROGRESS) {
        statusText.setString("Meci in desfasurare. " + echipaMea->getNume() + " serveste: " + (echipaMea->get_serve() ? "DA" : "NU"));
        simulatePoint();
    } else if (currentState == MATCH_END_DELAY) {
        statusText.setString("MECI TERMINAT: " + finalMessage);

        if (actionClock.getElapsedTime().asSeconds() > 3.0f) {
            currentState = MATCH_FINISHED;
        }
    } else if (currentState == TIMEOUT_CALLED) {
        statusText.setString("TIMEOUT CERUT. Se asteapta revenirea din menu...");
    }
}

void MatchLeague::handleInput(const sf::Event& event, sf::RenderWindow& ) {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = moved->position;
        if (currentState == SIMULATION_IN_PROGRESS) {
            timeoutButton.setHover(timeoutButton.isClicked(mousePos));
        } else if (currentState == MATCH_FINISHED) {
            continueButton.setHover(continueButton.isClicked(mousePos));
        }
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (currentState == SIMULATION_IN_PROGRESS) {
                if (timeoutButton.isClicked(mousePos)) {
                    currentState = TIMEOUT_CALLED;
                    this->next_screen_id = SCREEN_TIMEOUT;
                    return;
                }
            } else if (currentState == MATCH_FINISHED) {
                if (continueButton.isClicked(mousePos)) {
                    this->next_screen_id = SCREEN_LIGA;
                    return;
                }
            }
        }
    }
}

void MatchLeague::render(sf::RenderWindow& window) {
    window.clear(sf::Color(10, 50, 20));

    tablaScor.draw(window);
    window.draw(statusText);

    if (currentState == SIMULATION_IN_PROGRESS) {
        timeoutButton.draw(window);
    } else if (currentState == MATCH_FINISHED) {
        continueButton.draw(window);
    }
}


void MatchLeague::resetare_scoruri() {
    echipaMea->resetPunctaj();
    echipaMea->resetSetur();
    echipaAdversa->resetPunctaj();
    echipaAdversa->resetSetur();

    isMyTeamServing = true;
    echipaMea->serve_on();
    echipaAdversa->serve_off();
}

int MatchLeague::run(sf::RenderWindow& window) {
    this->next_screen_id = SCREEN_MATCH_LEAGUE;



    currentState = SIMULATION_IN_PROGRESS;

    while (window.isOpen() && next_screen_id == SCREEN_MATCH_LEAGUE) {

        if (currentState == TIMEOUT_CALLED) {
            currentState = SIMULATION_IN_PROGRESS;
            actionClock.restart();
        }

        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                return SCREEN_EXIT;
            }
            handleInput(event, window);
        }

        update();
        render(window);
        window.display();
    }

    return next_screen_id;
}