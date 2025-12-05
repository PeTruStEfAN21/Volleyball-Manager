#pragma once

#include "Screen.h"
#include "Echipe.h"
#include "BazaDeDate.h"
#include "Button.h"
#include "ListaJucatoriGui.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>

using Echipeptr = std::shared_ptr<Echipe>;
using BazaDeDateptr = std::shared_ptr<BazaDeDate>;

class InitialDraftScreen : public Screen {
public:
    InitialDraftScreen(Echipeptr echipa, BazaDeDateptr db, sf::Font& font);

    int run(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;

private:
    Echipeptr echipaMea;
    BazaDeDateptr baza;
    sf::Font& fontRef;

    long long bugetCurent;
    std::vector<jucatorptr> selectieTemporara;

    Button finalizeButton;
    sf::Text mesajStatus;
    ListaJucatoriGui listaVizuala;

    sf::Clock transitionClock;
    bool isTransitioning;
    void handlePlayerClick(sf::Vector2i mousePos);
    void handleFinalizeButton();
};