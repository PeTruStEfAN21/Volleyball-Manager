#include "MainMenuScreen.h"
#include <iostream>
#include <optional>

MainMenuScreen::MainMenuScreen(sf::Font& f)
    : transferButton(300.0f, 60.0f, 250.0f, 40.0f, "Transferuri", f),
      ligaButton(300.0f, 110.0f, 250.0f, 40.0f, "Start Liga", f),
      matchButton(300.0f, 160.0f, 250.0f, 40.0f, "Meci Test", f),
      firstSixButton(300.0f, 210.0f, 250.0f, 40.0f, "Alegere Echipa", f),
      historyMatchesButton(300.0f, 260.0f, 250.0f, 40.0f, "Istoric Meciuri", f),
      historyTransfersButton(300.0f, 310.0f, 250.0f, 40.0f, "Istoric Achizitii", f),
      packsButton(300.0f, 360.0f, 250.0f, 40.0f, "Magazin Pachete", f),
      exitButton(300.0f, 440.0f, 250.0f, 40.0f, "Iesire", f)
{
    // Culori personalizate pentru butoane specifice
    exitButton.setBaseColor(sf::Color(150, 50, 50));        // Rosu pentru iesire
    matchButton.setBaseColor(sf::Color(0, 100, 150));      // Albastru pentru meci
    packsButton.setBaseColor(sf::Color(184, 134, 11));     // Auriu pentru pachete

    // Culori pentru istorice
    historyMatchesButton.setBaseColor(sf::Color(70, 130, 180));
    historyTransfersButton.setBaseColor(sf::Color(70, 130, 180));
}

void MainMenuScreen::handleInput(const sf::Event& event, sf::RenderWindow& ) {
    // Gestionare HOVER (schimbare culoare la trecerea mouse-ului)
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = moved->position;

        transferButton.setHover(transferButton.isClicked(mousePos));
        ligaButton.setHover(ligaButton.isClicked(mousePos));
        matchButton.setHover(matchButton.isClicked(mousePos));
        firstSixButton.setHover(firstSixButton.isClicked(mousePos));
        historyMatchesButton.setHover(historyMatchesButton.isClicked(mousePos));
        historyTransfersButton.setHover(historyTransfersButton.isClicked(mousePos));
        packsButton.setHover(packsButton.isClicked(mousePos));
        exitButton.setHover(exitButton.isClicked(mousePos));
    }

    // Gestionare CLICK
    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (transferButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_TRANSFER;
            }
            else if (ligaButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_LIGA;
            }
            else if (matchButton.isClicked(mousePos)) {
                this->next_screen_id = MATCH_SCREEN;
            }
            else if (firstSixButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_FIRST_SIX;
            }
            else if (historyMatchesButton.isClicked(mousePos)) {
                std::cout << "[MENIU] Navigare: Istoric Meciuri" << std::endl;
                this->next_screen_id = 10; // Index stabilit in main
            }
            else if (historyTransfersButton.isClicked(mousePos)) {
                std::cout << "[MENIU] Navigare: Istoric Achizitii" << std::endl;
                this->next_screen_id = 11; // Index stabilit in main
            }
            else if (packsButton.isClicked(mousePos)) {
                std::cout << "[MENIU] Navigare: Magazin Pachete" << std::endl;
                this->next_screen_id = 13; // Index stabilit pentru PackScreen
            }
            else if (exitButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_EXIT;
            }
        }
    }
}

void MainMenuScreen::update() {
    // Poti adauga logica de update aici daca e nevoie
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(40, 40, 80)); // Fundal albastru inchis

    // Desenam toate butoanele
    transferButton.draw(window);
    ligaButton.draw(window);
    matchButton.draw(window);
    firstSixButton.draw(window);
    historyMatchesButton.draw(window);
    historyTransfersButton.draw(window);
    packsButton.draw(window);
    exitButton.draw(window);
}

int MainMenuScreen::run(sf::RenderWindow& window) {
    std::cout << "[MAIN MENU] Start run()" << std::endl;
    this->next_screen_id = SCREEN_MAIN_MENU;

    while (window.isOpen() && next_screen_id == SCREEN_MAIN_MENU) {
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

    std::cout << "[MAIN MENU] Returnez ID: " << next_screen_id << std::endl;
    return next_screen_id;
}