#pragma once

#include "Screen.h"
#include "Button.h"
#include "Echipe.h"
#include "ListaJucatoriGui.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>


class FirstSixScreen : public Screen {
private:
    Echipeptr echipaMea;
    sf::Font& fontRef;

    std::vector<jucatorptr> titulariSelectati;

    ListaJucatoriGui listaVizuala;

    Button startMatchButton;
    sf::Text titluText;
    sf::Text statusText;
    sf::Text infoText;


    void handleInput(const sf::Event& event, sf::RenderWindow& ) override;
    void handlePlayerClick(sf::Vector2i mousePos);
    void handleStartButton();
    void updateInfoText();
    bool isPositionFull(const std::string& pozitie);

public:
    FirstSixScreen(Echipeptr echipa, sf::Font& font);

    int run(sf::RenderWindow& window) override;

    void update_jucatori(const vector<jucatorptr>& jucatori);

    void render(sf::RenderWindow& window) override;
};