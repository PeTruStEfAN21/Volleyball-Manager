#pragma once
#include "Screen.h"
#include "Button.h" 
#include <SFML/Graphics.hpp>

class MainMenuScreen : public Screen {
private:
    sf::Font& font;

    Button firstSixButton;
    Button transferButton;
    Button ligaButton;
    Button matchButton;
    Button exitButton;

    bool ignoreNextClick = true;

    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void render(sf::RenderWindow& window) override;

public:
    MainMenuScreen(sf::Font& f);
    
    int run(sf::RenderWindow& window) override;
};