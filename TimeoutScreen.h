#pragma once

#include "Screen.h"
#include "Echipe.h"
#include "Button.h"
#include "BazaDeDate.h"
#include "ListaJucatoriGui.h"


constexpr int MATCH_SCREEN_ID = MATCH_SCREEN;

const float TEREN_X_START = 50.0f;
const float BANCA_X_START = 550.0f;
const float LISTA_Y_START = 100.0f;

class TimeoutScreen : public Screen {
private:
    Echipeptr echipaMea;
    BazaDeDateptr baza;
    sf::Font& fontRef;

    sf::Text mesajStatus;
    Button finalizeButton;
    Button continueButton;
    sf::Text labelTeren, labelBanca;

    int returnScreenId = SCREEN_MAIN_MENU;
    ListaJucatoriGui listaTeren;
    ListaJucatoriGui listaBanca;

    jucatorptr playerOut = nullptr;
    jucatorptr playerIn = nullptr;

    void updatePlayerLists();
    void handleFinalizeButton();
    void handlePlayerClick(sf::Vector2i mousePos);

public:

    void setReturnScreenId(int id) ;
    TimeoutScreen(Echipeptr echipa, BazaDeDateptr db, sf::Font& font);
    void handleInput(const sf::Event& event, sf::RenderWindow& window) override;
    void update() override;
    void render(sf::RenderWindow& window) override;
    int run(sf::RenderWindow& window) override;
};