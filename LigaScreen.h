#pragma once
#include "Screen.h"
#include "Button.h"
#include "Liga.h"
#include "Echipe.h"
#include "BazaDeDate.h"
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <string>

class BazaDeDate;
class manageri;
class jucator;

using BazaDeDateptr = std::shared_ptr<BazaDeDate>;
using Echipeptr = std::shared_ptr<Echipe>;

class LigaScreen : public Screen {
private:

    bool justFinishedMatch = false;

    sf::Font& fontRef;
    std::shared_ptr<Liga> liga;
    Echipeptr echipaMea;

    Button btnTabelaPunctaje;
    Button btnTransferuri;
    Button btnFirstSix;
    Button btnMeciLiga;
    Button btnExit;

    sf::Text statusText;
    sf::Text titleText;

    void updateButtonText();
    void handleInput(const sf::Event& event, sf::RenderWindow& ) override;

   // std::string formatScoreboard() const;

    sf::RenderWindow& windowRef;

public:
    LigaScreen(std::shared_ptr<Liga> currentLiga, Echipeptr playerTeam, BazaDeDateptr db, sf::Font& font, sf::RenderWindow& window);
    int run(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
    void update() override;
    void notifyMatchFinished() ;
};