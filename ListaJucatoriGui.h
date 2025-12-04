#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>


class jucator;
using jucatorptr = std::shared_ptr<jucator>;

class ListaJucatoriGui {
private:
    std::vector<jucatorptr> jucatoriAfisati;
    sf::Font& fontRef;
    float startX, startY;


    sf::FloatRect calculeaza_pozitie_rand(size_t index) const;

public:
    ListaJucatoriGui(sf::Font& font, const std::vector<jucatorptr>& listaInitiala, float x, float y);

    const std::vector<jucatorptr>& getLista() const;
    void setLista(const std::vector<jucatorptr>& listaNoua);
    void draw(sf::RenderWindow& window, const std::vector<jucatorptr>& playersHighlighted);
    void draw(sf::RenderWindow& window, jucatorptr playerHighlighted = nullptr);
    int getIndexFromClick(sf::Vector2i mousePos);
};