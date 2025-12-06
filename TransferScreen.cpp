#include "TransferScreen.h"
#include <iostream>
#include <string>
#include <optional>
#include <algorithm>
#include <random>
#include <chrono>


TransferScreen::TransferScreen(BazaDeDateptr baza, Echipeptr echipa, sf::Font& font)
    : baza(baza), echipaManager(echipa), fontRef(font),
      backButton(800.0f, 50.0f, 150.0f, 40.0f, "Inapoi", font),
      bugetText(font), messageText(font)
{
    bugetText.setCharacterSize(22);
    bugetText.setFillColor(sf::Color::Cyan);
    bugetText.setPosition(sf::Vector2f(50.0f, 50.0f));

    messageText.setCharacterSize(18);
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(sf::Vector2f(50.0f, 600.0f));

    refreshPlayerList();
}


int TransferScreen::run(sf::RenderWindow& window) {
    this->next_screen_id = SCREEN_TRANSFER;

    while (const std::optional<sf::Event> eventOpt = window.pollEvent()) { }

    while (window.isOpen() && next_screen_id == SCREEN_TRANSFER) {
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;
            if (event.is<sf::Event::Closed>()) return SCREEN_EXIT;
            handleInput(event, window);
        }
        update();
        render(window);
        window.display();
    }
    return next_screen_id;
}


void TransferScreen::refreshPlayerList() {
    playerRows.clear();
    hoverIndex = 1000;

    std::vector<jucatorptr> jucatoriDisponibili;
    for (const auto& jucator : baza->getLista()) {
        if (jucator->get_echipe() == nullptr) {
            jucatoriDisponibili.push_back(jucator);
        }
    }

    if (!jucatoriDisponibili.empty()) {
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);

        std::shuffle(jucatoriDisponibili.begin(), jucatoriDisponibili.end(), generator);
    }

    float currentY = LIST_START_Y;


    const size_t MAX_DISPLAY = 20;

    for (size_t i = 0; i < jucatoriDisponibili.size() && i < MAX_DISPLAY; ++i) {

        playerRows.emplace_back(jucatoriDisponibili[i], fontRef, LIST_START_X, currentY, ROW_WIDTH, ROW_HEIGHT);
        currentY += ROW_HEIGHT + 2.0f;
    }
}


void TransferScreen::handleInput(const sf::Event& event, sf::RenderWindow& ) {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = moved->position;
        backButton.setHover(backButton.isClicked(mousePos));

        hoverIndex = 1000;
        for (size_t i = 0; i < playerRows.size(); ++i) {
            if (playerRows[i].bgRect.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))) {
                hoverIndex = i;
                break;
            }
        }
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (backButton.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_MAIN_MENU;
            }
            else if (hoverIndex != 1000) {
                jucatorptr jucatorAles = playerRows[hoverIndex].player;

                if (echipaManager->get_buget() < jucatorAles->get_pret()) {
                    messageText.setString("Eroare: Buget insuficient pentru " + jucatorAles->get_nume() + "!");
                    messageText.setFillColor(sf::Color::Red);
                } else {
                    try {
                        echipaManager->adaugare_jucatorGUI(jucatorAles);
                        messageText.setString("SUCCES: L-ai cumparat pe " + jucatorAles->get_nume() + "!");
                        messageText.setFillColor(sf::Color::Green);

                        refreshPlayerList();

                    } catch (const std::runtime_error& e) {
                        messageText.setString("Eroare la transfer: " + std::string(e.what()));
                        messageText.setFillColor(sf::Color::Red);
                    }
                }
            }
        }
    }
}

void TransferScreen::update() {
    bugetText.setString("Buget Disponibil: " + std::to_string(echipaManager->get_buget()) + " lei");
}


void TransferScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 30, 40));

    window.draw(bugetText);
    window.draw(messageText);
    backButton.draw(window);

    for (size_t i = 0; i < playerRows.size(); ++i) {
        if (i == hoverIndex) {
            playerRows[i].bgRect.setFillColor(sf::Color(80, 80, 120));
        } else {
            playerRows[i].bgRect.setFillColor((i % 2 == 0) ? sf::Color(50, 50, 70) : sf::Color(45, 45, 65));
        }

        window.draw(playerRows[i].bgRect);
        window.draw(playerRows[i].text);
    }
}