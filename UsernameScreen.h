#pragma once
#include "Screen.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class UsernameScreen : public Screen {
private:
    sf::Font& font;
    sf::Text textPrompt;
    sf::Text textInput;
    std::string inputNume;
    sf::RectangleShape box;

    void handleInput(const sf::Event& event, sf::RenderWindow&) override;
    void update() override;
    void render(sf::RenderWindow& window) override;

public:
    explicit UsernameScreen(sf::Font& f);
    int run(sf::RenderWindow& window) override;
};