#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Echipe.h"

class TableScore {
private:
    Echipeptr echipa1, echipa2;
    sf::Vector2f position;



    sf::Text numeEchipa1, scorEchipa1Puncte, scorEchipa1Seturi;
    sf::Text numeEchipa2, scorEchipa2Puncte, scorEchipa2Seturi;






public:

    void setOpponent(Echipeptr newOpponent);

    TableScore(sf::Font& font, Echipeptr teamA, Echipeptr teamB, float x, float y);

    void updateScore();

    void draw(sf::RenderWindow& window);

    void setServingTeamHighlight(bool isEchipa1Serving);
};

