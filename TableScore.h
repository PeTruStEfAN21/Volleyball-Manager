#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Echipe.h"

class TableScore {
private:
    sf::Text numeEchipa1, scorEchipa1Puncte, scorEchipa1Seturi;
    sf::Text numeEchipa2, scorEchipa2Puncte, scorEchipa2Seturi;

    Echipeptr echipa1, echipa2;

    sf::Font& fontRef;

    sf::Vector2f position;



public:

    void setOpponent(Echipeptr newOpponent);

    TableScore(sf::Font& font, Echipeptr teamA, Echipeptr teamB, float x, float y);

    void updateScore();

    void draw(sf::RenderWindow& window);

    void setServingTeamHighlight(bool isEchipa1Serving);
};

