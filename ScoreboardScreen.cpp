#include "ScoreboardScreen.h"
#include "Screen.h"
#include <sstream>
#include <iostream>

#define ROW_HEIGHT 30.f
#define START_Y 100.f
#define START_X 50.f

ScoreboardScreen::ScoreboardScreen(std::shared_ptr<Liga> currentLiga, sf::Font& font)
    : fontRef(font), 
      liga(currentLiga),
      backButton(800.f, 600.f, 150.f, 40.f, "Inapoi la Liga", font),
      titleText(font)
{
    titleText.setFont(font);
    titleText.setString("Tabela de Punctaje a Ligii");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition(sf::Vector2f(50.f, 30.f));
}

void ScoreboardScreen::updateScoreDisplay() {
    scoreTexts.clear();
    auto clasament = liga->getSortedClasament();
    
    if (clasament.empty()) return;

    for (size_t i = 0; i < clasament.size(); ++i) {
        std::stringstream ss;
        ss << i + 1 << ". " 
           << clasament[i].nume 
           << " (Puncte: " << clasament[i].puncte << ")";
        
        sf::Text text(fontRef, ss.str(), 20);
        text.setFillColor((i == 0) ? sf::Color::Green : sf::Color::White);
        text.setPosition(sf::Vector2f(START_X, START_Y + i * ROW_HEIGHT));
        scoreTexts.push_back(text);
    }
}

void ScoreboardScreen::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        backButton.setHover(backButton.isClicked(moved->position));
    }
    
    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            if (backButton.isClicked(released->position)) {
                this->next_screen_id = SCREEN_LIGA;
            }
        }
    }
}



void ScoreboardScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(20, 20, 50)); 
    
    window.draw(titleText);
    backButton.draw(window);

    for (const auto& text : scoreTexts) {
        window.draw(text);
    }
}

int ScoreboardScreen::run(sf::RenderWindow& window) {
    this->next_screen_id = SCREEN_SCOREBOARD;
    
    updateScoreDisplay();

    while (window.isOpen() && this->next_screen_id == SCREEN_SCOREBOARD) {
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;
            if (event.is<sf::Event::Closed>()) return SCREEN_EXIT;
            handleInput(event, window);
        }

        update();
        render(window);
        window.display();
    }
    return this->next_screen_id;
}