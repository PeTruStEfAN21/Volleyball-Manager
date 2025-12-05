#pragma once
#include "Screen.h"
#include "Button.h"
#include "Liga.h" 
#include <SFML/Graphics.hpp>
#include <memory>

class ScoreboardScreen : public Screen {
private:
    sf::Font& fontRef;
    std::shared_ptr<Liga> liga; 

    Button backButton;
    sf::Text titleText;
    
    std::vector<sf::Text> scoreTexts;

    void updateScoreDisplay();
    void handleInput(const sf::Event& event, sf::RenderWindow& ) override;

public:
    ScoreboardScreen(std::shared_ptr<Liga> currentLiga, sf::Font& font);
    int run(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};