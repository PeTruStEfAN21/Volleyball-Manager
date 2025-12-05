#pragma once

#include "Screen.h"
#include "BazaDeDate.h"
#include "Echipe.h"
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>


using BazaDeDateptr = std::shared_ptr<BazaDeDate>;
using Echipeptr = std::shared_ptr<Echipe>;


struct PlayerRow {
    jucatorptr player;
    sf::RectangleShape bgRect;
    sf::Text text;

    PlayerRow(jucatorptr p, sf::Font& font, float x, float y, float width, float height) : player(p), text(font) {
        bgRect.setSize(sf::Vector2f(width, height));
        bgRect.setPosition(sf::Vector2f(x, y));
        bgRect.setFillColor(sf::Color(50, 50, 70));

        text.setCharacterSize(18);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(x + 10.0f, y + 5.0f));

        std::stringstream ss;
        ss << std::left << std::setw(18) << player->get_nume().substr(0, 17)
           << std::left << std::setw(5) << player->get_poz()
           << " OVR:" << std::setw(3) << player->get_ovr()
           << " | Pret: " << std::setw(8) << player->get_pret();
        text.setString(ss.str());
    }
};

class TransferScreen : public Screen {
public:
    TransferScreen(BazaDeDateptr baza, Echipeptr echipa, sf::Font& font);

    int run(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event, sf::RenderWindow& ) override;
    void update() override;
    void render(sf::RenderWindow& window) override;

private:
    BazaDeDateptr baza;
    Echipeptr echipaManager;
    sf::Font& fontRef;

    std::vector<PlayerRow> playerRows;
    size_t hoverIndex = -1;
    const float LIST_START_X = 50.0f;
    const float LIST_START_Y = 100.0f;
    const float ROW_HEIGHT = 35.0f;
    const float ROW_WIDTH = 700.0f;

    Button backButton;
    sf::Text bugetText;
    sf::Text messageText;

    void refreshPlayerList();
};