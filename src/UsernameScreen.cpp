#include "UsernameScreen.h"
#include "manageri.h"
#include "Echipe.h"
#include <iostream>

UsernameScreen::UsernameScreen(sf::Font& f)
    : font(f),
      textPrompt(f, "Introdu numele tau de Manager:", 30),
      textInput(f, "", 40),
      inputNume(""),
      box(sf::Vector2f(400.0f, 60.0f))
{
    (void)font;
    textPrompt.setPosition({250.0f, 200.0f});

    textInput.setFillColor(sf::Color::Yellow);
    textInput.setPosition({300.0f, 300.0f});

    box.setPosition({290.0f, 295.0f});
    box.setFillColor(sf::Color(50, 50, 50));
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(sf::Color::White);
}

void UsernameScreen::handleInput(const sf::Event& event, sf::RenderWindow& ) {
    if (const auto* textEv = event.getIf<sf::Event::TextEntered>()) {
        if (textEv->unicode == 8) {
            if (!inputNume.empty()) inputNume.pop_back();
        }
        else if (textEv->unicode == 13) {
            if (!inputNume.empty()) {
                auto echipa = manageri::getInstance().getEchipaManager();
                if (echipa != nullptr) {
                    echipa->set_nume(inputNume);
                    std::cout << "[DEBUG] Nume setat: " << inputNume << std::endl;
                    this->next_screen_id = SCREEN_DRAFT;
                } else {
                    std::cerr << "[EROARE] Echipa managerului nu este initializata!" << std::endl;
                }
            }
        }
        else if (textEv->unicode < 128 && textEv->unicode >= 32) {
            inputNume += static_cast<char>(textEv->unicode);
        }
    }
}

void UsernameScreen::update() {
    textInput.setString(inputNume + "|");
}

void UsernameScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(40, 40, 80));
    window.draw(textPrompt);
    window.draw(box);
    window.draw(textInput);
}

int UsernameScreen::run(sf::RenderWindow& window) {
    this->next_screen_id = 12;

    while (window.isOpen() && next_screen_id == 12) {
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            if (eventOpt->is<sf::Event::Closed>()) return SCREEN_EXIT;
            handleInput(*eventOpt, window);
        }
        update();
        render(window);
        window.display();
    }
    return next_screen_id;
}