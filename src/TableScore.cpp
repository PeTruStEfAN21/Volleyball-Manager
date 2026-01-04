#include "TableScore.h"
#include "Echipe.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdexcept>


TableScore::TableScore(sf::Font &font, Echipeptr teamA, Echipeptr teamB, float x, float y)

: echipa1(teamA),
  echipa2(teamB),
  position(x, y),
  numeEchipa1(font),
  scorEchipa1Puncte(font),
  scorEchipa1Seturi(font),
  numeEchipa2(font),
  scorEchipa2Puncte(font),
  scorEchipa2Seturi(font)
{
    const float SPACING = 150.0f;
    const float VERTICAL_OFFSET_SCORE = 30.0f;

    numeEchipa1.setCharacterSize(24);
    numeEchipa1.setPosition(sf::Vector2f(position.x, position.y));
    numeEchipa1.setFillColor(sf::Color::Yellow);
    numeEchipa1.setString(echipa1->getNume());

    scorEchipa1Puncte.setCharacterSize(40);
    scorEchipa1Puncte.setPosition(sf::Vector2f(position.x + SPACING, position.y - 10.0f));
    scorEchipa1Puncte.setFillColor(sf::Color::White);
    scorEchipa1Puncte.setString(std::to_string(echipa1->getPunctaj()));

    scorEchipa1Seturi.setCharacterSize(18);
    scorEchipa1Seturi.setPosition(sf::Vector2f(position.x, position.y + VERTICAL_OFFSET_SCORE));
    scorEchipa1Seturi.setFillColor(sf::Color(200, 200, 200));
    scorEchipa1Seturi.setString("Seturi: " + std::to_string(echipa1->getSeturi()));


    numeEchipa2.setCharacterSize(24);
    numeEchipa2.setPosition(sf::Vector2f(position.x + SPACING * 3, position.y));
    numeEchipa2.setFillColor(sf::Color::Blue);
    numeEchipa2.setString(echipa2->getNume());

    scorEchipa2Puncte.setCharacterSize(40);
    scorEchipa2Puncte.setPosition(sf::Vector2f(position.x + SPACING * 2, position.y - 10.0f));
    scorEchipa2Puncte.setFillColor(sf::Color::White);
    scorEchipa2Puncte.setString(std::to_string(echipa2->getPunctaj()));

    scorEchipa2Seturi.setCharacterSize(18);
    scorEchipa2Seturi.setPosition(sf::Vector2f(position.x + SPACING * 3, position.y + VERTICAL_OFFSET_SCORE));
    scorEchipa2Seturi.setFillColor(sf::Color(200, 200, 200));
    scorEchipa2Seturi.setString("Seturi: " + std::to_string(echipa2->getSeturi()));

}

void TableScore::updateScore() {
    scorEchipa1Puncte.setString(std::to_string(echipa1->getPunctaj()));
    scorEchipa2Puncte.setString(std::to_string(echipa2->getPunctaj()));

    scorEchipa1Seturi.setString("Seturi: " + std::to_string(echipa1->getSeturi()));
    scorEchipa2Seturi.setString("Seturi: " + std::to_string(echipa2->getSeturi()));
}


void TableScore::draw(sf::RenderWindow& window) {
    window.draw(numeEchipa1);
    window.draw(scorEchipa1Puncte);
    window.draw(scorEchipa1Seturi);

    window.draw(numeEchipa2);
    window.draw(scorEchipa2Puncte);
    window.draw(scorEchipa2Seturi);
}

void TableScore::setOpponent(Echipeptr newOpponent) {
    if (!newOpponent) return;

    echipa2 = newOpponent;

    numeEchipa2.setString(echipa2->getNume());

    updateScore();
}

void TableScore::setServingTeamHighlight(bool isEchipa1) {
    if (isEchipa1) {
        numeEchipa2.setFillColor(sf::Color::Blue);
        numeEchipa1.setFillColor(sf::Color::Green);

    }
    else {
        numeEchipa1.setFillColor(sf::Color::Yellow);
        numeEchipa2.setFillColor(sf::Color::Green);
    }
}