#pragma once
#include "Screen.h"
#include "Button.h" 
#include <SFML/Graphics.hpp>

class MainMenuScreen : public Screen {
private:
    sf::Font& font;

    Button transferButton;
    Button ligaButton;
    Button matchButton;
    Button firstSixButton;
    Button exitButton;
    void handleInput(const sf::Event& event, sf::RenderWindow& ) override;
    void update() override;
    void render(sf::RenderWindow& window) override;

public:
    explicit MainMenuScreen(sf::Font& f);
    
    int run(sf::RenderWindow& window) override;
};