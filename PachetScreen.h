#pragma once
#include "Screen.h"
#include "Button.h"
#include "Pachete.h"
#include "Echipe.h"

class PackScreen : public Screen {
private:
    sf::Font& font;
    Button btnBronze, btnGold, btnBack;
    sf::Text textStatus, textCastig;
    Echipeptr echipaMea;
    std::vector<jucatorptr> bazaJucatori;
    jucatorptr ultimulCastigat;
    Gestiune<jucatorptr>& istoricRef;


public:
    PackScreen(sf::Font& f, Echipeptr ech, const std::vector<jucatorptr>& b,Gestiune<jucatorptr>& ist) ;

    int run(sf::RenderWindow& window) override ;

    void handleInput(const sf::Event&, sf::RenderWindow&) override ;
    void update() override ;
    void render(sf::RenderWindow&) override ;
};