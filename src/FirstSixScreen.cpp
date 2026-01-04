#include "FirstSixScreen.h"
#include <iostream>
#include <optional>
#include <algorithm>
#include "Exceptii.h"

#include "BazaDeDate.h"

const sf::Vector2f LIST_POS(50.0f, 100.0f);
const sf::Vector2f INFO_POS(800.0f, 100.0f);

FirstSixScreen::FirstSixScreen(Echipeptr echipa, sf::Font& font)
    : echipaMea(echipa),
      listaVizuala(font, echipaMea->get_jucatori(), LIST_POS.x, LIST_POS.y),
      startMatchButton(800.0f, 600.0f, 220.0f, 50.0f, "Start Meci", font),
      titluText(font), statusText(font), infoText(font)
{

    titluText.setFont(font);
    titluText.setString("Alege Titularii (Primii 6)");
    titluText.setCharacterSize(30);
    titluText.setPosition(sf::Vector2f(50.0f, 30.0f));
    titluText.setFillColor(sf::Color::White);

    statusText.setFont(font);
    statusText.setCharacterSize(18);
    statusText.setPosition(sf::Vector2f(50.0f, 650.0f));
    statusText.setFillColor(sf::Color::Cyan);
    statusText.setString("Selecteaza jucatorii pentru a incepe.");

    infoText.setFont(font);
    infoText.setCharacterSize(20);
    infoText.setPosition(INFO_POS);
    infoText.setFillColor(sf::Color::White);

    updateInfoText();
}

int FirstSixScreen::run(sf::RenderWindow& window) {

    next_screen_id = SCREEN_FIRST_SIX;


    while (window.isOpen() && next_screen_id == SCREEN_FIRST_SIX) {

        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                return SCREEN_EXIT;
            }
            handleInput(event, window);
        }

        update();
        render(window);
        window.display();
    }
    return next_screen_id;
}
void FirstSixScreen::handleInput(const sf::Event& event, sf::RenderWindow& ) {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = moved->position;
        startMatchButton.setHover(startMatchButton.isClicked(mousePos));
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (startMatchButton.isClicked(mousePos)) {
                handleStartButton();
            } else {
                handlePlayerClick(mousePos);
            }
        }
    }
}

bool FirstSixScreen::isPositionFull(const std::string& pozitie) {
    int max = 0;
    if (pozitie == "Setter") max = 1;
    else if (pozitie == "OutsideHitter") max = 2;
    else if (pozitie == "MiddleBlocker") max = 1;
    else if (pozitie == "OppositeHitter") max = 1;
    else if (pozitie == "Libero") max = 1;

    int count = 0;
    for (const auto& j : titulariSelectati) {
        if (j->get_poz() == pozitie) count++;
    }
    return count >= max;
}

void FirstSixScreen::handlePlayerClick(sf::Vector2i mousePos) {
    int index = listaVizuala.getIndexFromClick(mousePos);
    if (index == -1) return;

    auto totiJucatorii = echipaMea->get_jucatori();
    if (index >= (int)totiJucatorii.size()) return;

    jucatorptr jucatorAles = totiJucatorii[index];

    auto it = std::find(titulariSelectati.begin(), titulariSelectati.end(), jucatorAles);

    if (it != titulariSelectati.end()) {
        titulariSelectati.erase(it);
        statusText.setString("Jucator scos din echipa de start.");
statusText.setFillColor(sf::Color::White);
    } else {
        if (titulariSelectati.size() >= 6) {
            statusText.setString("Ai selectat deja 6 jucatori! Scoate unul pentru a-l inlocui.");
            statusText.setFillColor(sf::Color::Red);
            return;
        }

        if (isPositionFull(jucatorAles->get_poz())) {
            statusText.setString("Pozitia " + jucatorAles->get_poz() + " este deja ocupata complet!");
            statusText.setFillColor(sf::Color(255, 100, 100));
            return;
        }

        titulariSelectati.push_back(jucatorAles);
        statusText.setString("Jucator adaugat la titulari.");
        statusText.setFillColor(sf::Color::Green);
    }

    updateInfoText();
}

void FirstSixScreen::handleStartButton() {
    try {
        float ovr = echipaMea->valideaza_si_set_start_6(titulariSelectati);

        statusText.setString("Succes! Overall Start 6: " + std::to_string(ovr));
        statusText.setFillColor(sf::Color::Green);

         this->next_screen_id = SCREEN_MAIN_MENU;

    }catch (const JocLogicException& e) {
        statusText.setString("Eroare: " + std::string(e.what()));
        statusText.setFillColor(sf::Color::Red);
    }
}
void FirstSixScreen::updateInfoText() {
    std::map<std::string, int> counts;
    counts["Setter"] = 0;
    counts["OutsideHitter"] = 0;
    counts["MiddleBlocker"] = 0;
    counts["OppositeHitter"] = 0;
    counts["Libero"] = 0;

    for (const auto& j : titulariSelectati) {
        counts[j->get_poz()]++;
    }

    std::string s = "Formatie Necesara:\n\n";
    s += "Setter: " + std::to_string(counts["Setter"]) + "/1\n";
    s += "Outside Hitter: " + std::to_string(counts["OutsideHitter"]) + "/2\n";
    s += "Middle Blocker: " + std::to_string(counts["MiddleBlocker"]) + "/1\n";
    s += "Opposite Hitter: " + std::to_string(counts["OppositeHitter"]) + "/1\n";
    s += "Libero: " + std::to_string(counts["Libero"]) + "/1\n";

    infoText.setString(s);
}



void FirstSixScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 30, 40));

    window.draw(titluText);


    listaVizuala.draw(window, titulariSelectati);

    window.draw(infoText);
    window.draw(statusText);
    startMatchButton.draw(window);
}



void FirstSixScreen::update_jucatori(const vector<jucatorptr>& jucatori) {
    listaVizuala.setLista(jucatori);
}
