#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

enum ScreenState {
    SCREEN_EXIT = -1,
    SCREEN_MAIN_MENU = 0,
    SCREEN_TRANSFER = 1,
    SCREEN_LIGA = 2,
    SCREEN_DRAFT = 3,
    MATCH_SCREEN = 4,
    SCREEN_MATCH = 5,
    SCREEN_TIMEOUT = 6,
    SCREEN_FIRST_SIX = 7,
    SCREEN_SCOREBOARD = 8,
    SCREEN_MATCH_LEAGUE = 9

};

class Screen {
protected:

    int next_screen_id = -1;

    static std::shared_ptr<sf::RenderTexture> globalFont;


public:
    virtual int run(sf::RenderWindow& window) = 0; 
    
    virtual ~Screen() = default;

    virtual void render(sf::RenderWindow& ){};

    virtual void update(){};

    virtual void handleInput(const sf::Event& , sf::RenderWindow& ){};

    static sf::Font& getGlobalFont();


};

using Screenptr = std::shared_ptr<Screen>;