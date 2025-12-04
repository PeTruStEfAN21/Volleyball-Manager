#include "MainMenuScreen.h"
#include <iostream>
#include <optional>

MainMenuScreen::MainMenuScreen(sf::Font& f)
    : font(f),
      transferButton(300.0f, 120.0f, 200.0f, 50.0f, "Transferuri", f),
      ligaButton(300.0f, 210.0f, 200.0f, 50.0f, "Start Liga", f),
      matchButton(300.0f, 300.0f, 200.0f, 50.0f, "Meci Test", f),
      firstSixButton(300.0f, 390.0f, 200.0f, 50.0f, "Alegere Echipa", f),
      exitButton(300.0f, 480.0f, 200.0f, 50.0f, "Iesire", f)

{
    exitButton.setBaseColor(sf::Color(150, 50, 50));
    matchButton.setBaseColor(sf::Color(0, 100, 150));
}

void MainMenuScreen::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = moved->position;
        transferButton.setHover(transferButton.isClicked(mousePos));
        ligaButton.setHover(ligaButton.isClicked(mousePos));
        matchButton.setHover(matchButton.isClicked(mousePos));
        firstSixButton.setHover(firstSixButton.isClicked(mousePos));
        exitButton.setHover(exitButton.isClicked(mousePos));
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (transferButton.isClicked(mousePos)) {
                std::cout << "[MENIU] Click pe Transfer -> Merg la SCREEN_TRANSFER" << std::endl;
                this->next_screen_id = SCREEN_TRANSFER;
            }
            else if (ligaButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_LIGA;
            }
            else if (matchButton.isClicked(mousePos)) {
                this->next_screen_id = MATCH_SCREEN;
            }
            else if (exitButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_EXIT;
            }
            else if (firstSixButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_FIRST_SIX ;
            }
        }
    }
}

void MainMenuScreen::update() {
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(40, 40, 80));
    transferButton.draw(window);
    ligaButton.draw(window);
    matchButton.draw(window);
    firstSixButton.draw(window);
    exitButton.draw(window);
}

int MainMenuScreen::run(sf::RenderWindow& window) {
    std::cout << "[MAIN MENU] Start run()" << std::endl;

    this->next_screen_id = SCREEN_MAIN_MENU;

    while (window.isOpen() && next_screen_id == SCREEN_MAIN_MENU) {

        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                std::cout << "[MAIN MENU] Fereastra inchisa." << std::endl;
                return SCREEN_EXIT;
            }

            handleInput(event, window);
        }

        update();
        render(window);
        window.display();
    }

    std::cout << "[MAIN MENU] Stop run(). Returnez ID: " << next_screen_id << std::endl;
    return next_screen_id;
}