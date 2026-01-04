#include "TimeoutScreen.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <optional>
#include <chrono>
#include "Exceptii.h"





TimeoutScreen::TimeoutScreen(Echipeptr echipa, BazaDeDateptr db, sf::Font& font)
    : echipaMea(echipa),
      baza(db),

      listaTeren(font, echipaMea->get_jucatori_pe_teren(), TEREN_X_START, LISTA_Y_START),
      listaBanca(font, echipaMea->get_jucatori_de_pe_banca(), BANCA_X_START, LISTA_Y_START),

      finalizeButton(400.0f, 650.0f, 200.0f, 40.0f, "Asteapta Selectie...", font),
      continueButton(550.0f, 650.0f, 200.0f, 40.0f, "Continua Meciul", font),
      mesajStatus(font),
      labelTeren(font),
      labelBanca(font)
{

    mesajStatus.setCharacterSize(16);
    mesajStatus.setPosition(sf::Vector2f(50.0f, 650.0f));
    mesajStatus.setFillColor(sf::Color::White);
    mesajStatus.setString("Selecteaza jucatorul din teren (stanga) pe care doresti sa il schimbi.");

    labelTeren.setFont(font);
    labelTeren.setCharacterSize(20);
    labelTeren.setString("Jucatori in Teren");
    labelTeren.setPosition(sf::Vector2f(TEREN_X_START, LISTA_Y_START - 40.0f));

    labelBanca.setFont(font);
    labelBanca.setCharacterSize(20);
    labelBanca.setString("Jucatori pe Banca");
    labelBanca.setPosition(sf::Vector2f(BANCA_X_START, LISTA_Y_START - 40.0f));

    continueButton.setBaseColor(sf::Color(100, 50, 150));
    finalizeButton.setBaseColor(sf::Color(80, 80, 80));
}



void TimeoutScreen::handleInput(const sf::Event& event, sf::RenderWindow& ) {

    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        continueButton.setHover(continueButton.isClicked(moved->position));

        if (playerOut && playerIn) {
            finalizeButton.setHover(finalizeButton.isClicked(moved->position));
        } else {
            finalizeButton.setHover(false);
        }
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {

            if (continueButton.isClicked(released->position)) {
                this->next_screen_id = returnScreenId;
                return;
            }

            if (finalizeButton.isClicked(released->position) && playerOut && playerIn) {
                handleFinalizeButton();
            }

            else {
                handlePlayerClick(released->position);
            }
        }
    }
}

void TimeoutScreen::setReturnScreenId(int id) { returnScreenId = id; }

void TimeoutScreen::handlePlayerClick(sf::Vector2i mousePos) {
    mesajStatus.setFillColor(sf::Color::White);

    int indexTeren = listaTeren.getIndexFromClick(mousePos);
    int indexBanca = listaBanca.getIndexFromClick(mousePos);

    if (playerOut == nullptr || indexTeren != -1) {


        if (indexTeren != -1) {
            playerOut = listaTeren.getLista()[indexTeren];
            playerIn = nullptr;

            mesajStatus.setString("Jucator din teren selectat: " + playerOut->get_nume() +
                                  " (" + playerOut->get_poz() + "). Acum selecteaza jucatorul de pe banca.");

            finalizeButton.setString("Asteapta Selectie...");
            finalizeButton.setBaseColor(sf::Color(80, 80, 80));
        } else if (indexBanca != -1) {
            mesajStatus.setString("Eroare: Incepe selectia cu un jucator DIN TEREN (stanga).");
            mesajStatus.setFillColor(sf::Color::Red);
        }
    }
    else if (playerOut != nullptr) {
        if (indexBanca != -1) {
            jucatorptr playerInCandidate = listaBanca.getLista()[indexBanca];

            if (playerInCandidate->get_poz() != playerOut->get_poz()) {
                 mesajStatus.setString("Eroare de pozitie. " + playerOut->get_nume() + " (OUT) necesita un jucator de pozitia: " + playerOut->get_poz());
                 mesajStatus.setFillColor(sf::Color::Red);
                 playerIn = nullptr;
                 return;
            }

            playerIn = playerInCandidate;
            mesajStatus.setString("Schimbare valida: " + playerOut->get_nume() + " OUT cu " + playerIn->get_nume() + " IN. Apasa FINALIZARE.");
            mesajStatus.setFillColor(sf::Color(0, 255, 0));

            finalizeButton.setString("FINALIZARE SCHIMBARE");
            finalizeButton.setBaseColor(sf::Color(50, 180, 50));

        } else {
             mesajStatus.setString("Selecteaza jucatorul IN de pe BANCA (dreapta).");
             mesajStatus.setFillColor(sf::Color::Red);
        }
    }
}


void TimeoutScreen::handleFinalizeButton() {
    if (!playerOut || !playerIn) return;

    try {

        echipaMea->schimba_jucator_pe_teren(playerOut, playerIn);

        mesajStatus.setString("Schimbare efectuata: " + playerOut->get_nume() + " (OUT) cu " + playerIn->get_nume() + " (IN).");
        mesajStatus.setFillColor(sf::Color(0, 255, 0));

        updatePlayerLists();

        playerOut = nullptr;
        playerIn = nullptr;
        finalizeButton.setString("Asteapta Selectie...");
        finalizeButton.setBaseColor(sf::Color(80, 80, 80));

    } catch (const JocLogicException& e) {
        mesajStatus.setString("EROARE LA SCHIMBARE: " + std::string(e.what()));
        mesajStatus.setFillColor(sf::Color::Red);
    }
}




void TimeoutScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 30, 40));

    window.draw(labelTeren);
    window.draw(labelBanca);

    listaTeren.draw(window, playerOut);
    listaBanca.draw(window, playerIn);

    finalizeButton.draw(window);
    continueButton.draw(window);
    window.draw(mesajStatus);
}

int TimeoutScreen::run(sf::RenderWindow& window) {
    this->next_screen_id = SCREEN_TIMEOUT;

    updatePlayerLists();

    while (window.isOpen() && next_screen_id == SCREEN_TIMEOUT) {


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
void TimeoutScreen::updatePlayerLists() {
    std::vector<jucatorptr> jucatoriTeren;

    jucatoriTeren = echipaMea->get_jucatori_pe_teren();

    std::vector<jucatorptr> jucatoriBanca;

    jucatoriBanca = echipaMea->get_jucatori_de_pe_banca();

    listaTeren.setLista(jucatoriTeren);
    listaBanca.setLista(jucatoriBanca);

    playerOut = nullptr;
    playerIn = nullptr;
    finalizeButton.setString("Asteapta Selectie...");
    finalizeButton.setBaseColor(sf::Color(80, 80, 80));
    mesajStatus.setString("Selecteaza jucatorul din teren (stanga) pe care doresti sa il schimbi.");
    mesajStatus.setFillColor(sf::Color::White);
}