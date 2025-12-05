#include "LigaScreen.h"
#include "Screen.h"
#include <iostream>
#include <optional>
#include <sstream>
#include <algorithm>
#include <cstdlib>



LigaScreen::LigaScreen(std::shared_ptr<Liga> currentLiga, Echipeptr playerTeam, BazaDeDateptr db, sf::Font& font, sf::RenderWindow& window)
    : echipaMea(playerTeam),
      fontRef(font),
      liga(currentLiga),
      windowRef(window),

      btnTabelaPunctaje(50.0f, 100.0f, 300.0f, 50.0f, "1. Tabela de Punctaje", font),
      btnTransferuri(50.0f, 170.0f, 300.0f, 50.0f, "2. Lista de Transferuri", font),
      btnFirstSix(50.0f, 240.0f, 300.0f, 50.0f, "3. Echipa de Start (First Six)", font),
      btnMeciLiga(50.0f, 310.0f, 300.0f, 50.0f, "4. Joaca Urmatorul Meci", font),
      btnExit(50.0f, 380.0f, 300.0f, 50.0f, "5. Iesi la Meniul Principal", font),

      titleText(font),
      statusText(font)
{
    titleText.setString("Meniul Ligii");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition(sf::Vector2f(50.0f, 30.0f));

    statusText.setCharacterSize(18);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(sf::Vector2f(400.0f, 100.0f));
    statusText.setString("Bine ai venit in Liga!");

    if (liga->getEchipe().empty()) {
        liga->adaugare_echipa(echipaMea);
        liga->creare_liga();
    }
}

/*std::string LigaScreen::formatScoreboard() const {
    const auto& echipe_lista = liga->getEchipe();

    std::stringstream ss;
    ss << "=== Clasament Puncte Curent ===\n";

    if (echipe_lista.empty()) {
        ss << "Liga nu este inca formata.";
        return ss.str();
    }


    liga->afisare_punctaje();
    ss << "Punctajele au fost afisate in consola. \nVezi ordinea actuala a echipelor.";
    return ss.str();
}*/

void LigaScreen::updateButtonText() {
    if (!liga->isSeasonFinished()) {
        btnMeciLiga.setString("4. Joaca Urmatorul Meci");
    } else {
        btnMeciLiga.setString("Sezonul Ligii s-a terminat!");
    }
}



void LigaScreen::handleInput(const sf::Event& event, sf::RenderWindow& window) {

    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = moved->position;
        btnTabelaPunctaje.setHover(btnTabelaPunctaje.isClicked(mousePos));
        btnTransferuri.setHover(btnTransferuri.isClicked(mousePos));
        btnFirstSix.setHover(btnFirstSix.isClicked(mousePos));
        btnMeciLiga.setHover(btnMeciLiga.isClicked(mousePos));
        btnExit.setHover(btnExit.isClicked(mousePos));
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (btnTabelaPunctaje.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_SCOREBOARD;
            }
            else if (btnTransferuri.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_TRANSFER;
            }
            else if (btnFirstSix.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_FIRST_SIX;
            }
            else if (btnMeciLiga.isClicked(mousePos)) {
                if (!liga->isSeasonFinished()) {
                    this->next_screen_id = SCREEN_MATCH_LEAGUE;
                    statusText.setString("Trecere la meciul de Liga. Revino aici pentru a simula AI-ul.");
                } else {
                    statusText.setString("Sezonul Ligii s-a terminat! Nu mai poti juca meciuri.");
                }
            }
            else if (btnExit.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_MAIN_MENU;
            }
        }
    }
}


void LigaScreen::notifyMatchFinished() { justFinishedMatch = true; }


int LigaScreen::run(sf::RenderWindow& window) {
    this->next_screen_id = SCREEN_LIGA;

    liga->reset_etapa();

    if (justFinishedMatch) {
        statusText.setString("Rezultat inregistrat! Scorul a fost adaugat in clasament.");
        justFinishedMatch = false;
    }

    while (window.isOpen() && this->next_screen_id == SCREEN_LIGA) {

        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;
            if (event.is<sf::Event::Closed>()) return SCREEN_EXIT;
            handleInput(event, window);
        }

        update();
        render(window);
        window.display();
    }
    return this->next_screen_id;
}

void LigaScreen::update() {
    updateButtonText();

    sf::Vector2i mousePos = sf::Mouse::getPosition();
    btnTabelaPunctaje.setHover(btnTabelaPunctaje.isClicked(mousePos));
    btnTransferuri.setHover(btnTransferuri.isClicked(mousePos));
    btnFirstSix.setHover(btnFirstSix.isClicked(mousePos));
    btnMeciLiga.setHover(btnMeciLiga.isClicked(mousePos));
    btnExit.setHover(btnExit.isClicked(mousePos));
}

void LigaScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 30, 60));

    window.draw(titleText);

    btnTabelaPunctaje.draw(window);
    btnTransferuri.draw(window);
    btnFirstSix.draw(window);
    btnMeciLiga.draw(window);
    btnExit.draw(window);

    window.draw(statusText);
}