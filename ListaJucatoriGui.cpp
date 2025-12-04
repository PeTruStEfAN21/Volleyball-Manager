#include "ListaJucatoriGui.h"
#include "jucator.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

const float LINE_HEIGHT = 24.0f;
const float LINE_WIDTH = 450.0f;
const int MAX_VISIBLE_PLAYERS = 16;

sf::FloatRect ListaJucatoriGui::calculeaza_pozitie_rand(size_t index) const {
    sf::Vector2f position(
        startX,
        startY + (float)index * LINE_HEIGHT
    );

    sf::Vector2f size(
        LINE_WIDTH,
        LINE_HEIGHT
    );

    return sf::FloatRect(position, size);
}

ListaJucatoriGui::ListaJucatoriGui(sf::Font& font, const std::vector<jucatorptr>& listaInitiala, float x, float y)
    : fontRef(font),
      startX(x),
      startY(y)
{
    size_t limit = std::min(listaInitiala.size(), (size_t)MAX_VISIBLE_PLAYERS);
    jucatoriAfisati.assign(listaInitiala.begin(), listaInitiala.begin() + limit);
}


const std::vector<jucatorptr>& ListaJucatoriGui::getLista() const {
    return jucatoriAfisati;
}

void ListaJucatoriGui::setLista(const std::vector<jucatorptr>& listaNoua) {
    size_t limit = std::min(listaNoua.size(), (size_t)MAX_VISIBLE_PLAYERS);
    jucatoriAfisati.assign(listaNoua.begin(), listaNoua.begin() + limit);
}


void ListaJucatoriGui::draw(sf::RenderWindow& window, jucatorptr playerHighlighted) {

    for (size_t i = 0; i < jucatoriAfisati.size(); ++i) {
        jucatorptr j = jucatoriAfisati[i];
        sf::FloatRect randRect = calculeaza_pozitie_rand(i);

        sf::RectangleShape fundal;
        fundal.setSize(sf::Vector2f(randRect.size.x, randRect.size.y));
        fundal.setPosition(sf::Vector2f(randRect.position.x, randRect.position.y));

        sf::Color culoareFundal = (i % 2 == 0) ? sf::Color(40, 40, 50) : sf::Color(50, 50, 60);

        if (j == playerHighlighted) {
            culoareFundal = sf::Color(100, 100, 0);
        }

        fundal.setFillColor(culoareFundal);
        window.draw(fundal);

        sf::Text text(fontRef);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(randRect.position.x + 5.0f, randRect.position.y + 3.0f));

        std::stringstream ss;
        ss << std::left << std::setw(20) << j->get_nume();
        ss << std::left << std::setw(15) << j->get_poz();
        ss << std::left << std::setw(10) << j->get_pret();
        ss << "OVR: " << std::fixed << std::setprecision(1) << j->get_ovr();

        text.setString(ss.str());
        window.draw(text);
    }
}

int ListaJucatoriGui::getIndexFromClick(sf::Vector2i mousePos) {

    for (size_t i = 0; i < jucatoriAfisati.size(); ++i) {
        sf::FloatRect bounds = calculeaza_pozitie_rand(i);

        if (bounds.contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) {
            return (int)i;
        }
    }

    return -1;
}

void ListaJucatoriGui::draw(sf::RenderWindow& window, const std::vector<jucatorptr>& playersHighlighted) {

    for (size_t i = 0; i < jucatoriAfisati.size(); ++i) {
        jucatorptr j = jucatoriAfisati[i];
        sf::FloatRect randRect = calculeaza_pozitie_rand(i);

        sf::RectangleShape fundal;
        fundal.setSize(sf::Vector2f(randRect.size.x, randRect.size.y));
        fundal.setPosition(sf::Vector2f(randRect.position.x, randRect.position.y));

        sf::Color culoareFundal = (i % 2 == 0) ? sf::Color(40, 40, 50) : sf::Color(50, 50, 60);

        auto it = std::find(playersHighlighted.begin(), playersHighlighted.end(), j);
        if (it != playersHighlighted.end()) {
            culoareFundal = sf::Color(50, 150, 50); // Verde pentru titulari
        }

        fundal.setFillColor(culoareFundal);
        window.draw(fundal);

        sf::Text text(fontRef);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(randRect.position.x + 5.0f, randRect.position.y + 3.0f));

        std::stringstream ss;
        ss << std::left << std::setw(20) << j->get_nume();
        ss << std::left << std::setw(15) << j->get_poz();
        ss << std::left << std::setw(10) << j->get_pret();
        ss << "OVR: " << std::fixed << std::setprecision(1) << j->get_ovr();

        text.setString(ss.str());
        window.draw(text);
    }
}