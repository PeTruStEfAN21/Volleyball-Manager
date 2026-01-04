#include "PachetScreen.h"
#include <iostream>
#include <optional>


PackScreen::PackScreen(sf::Font& f, Echipeptr ech, const std::vector<jucatorptr>& b, Gestiune<jucatorptr>& ist)
    : font(f),
      btnBronze(100.0f, 200.0f, 250.0f, 100.0f, "Bronze Pack (5000)", f),
      btnGold(450.0f, 200.0f, 250.0f, 100.0f, "Gold Pack (25000)", f),
      btnBack(350.0f, 550.0f, 200.0f, 50.0f, "Inapoi", f),
      textStatus(f, "", 24),
      textCastig(f, "", 35),
      echipaMea(ech),
      bazaJucatori(b),
      ultimulCastigat(nullptr),
      istoricRef(ist)
{
    (void)font;
    textStatus.setPosition({50.0f, 50.0f});
    textStatus.setFillColor(sf::Color::White);

    textCastig.setPosition({200.0f, 400.0f});
    textCastig.setFillColor(sf::Color::Cyan);

    btnBronze.setBaseColor(sf::Color(139, 69, 19));
    btnGold.setBaseColor(sf::Color(218, 165, 32));
}

int PackScreen::run(sf::RenderWindow& window) {
    this->next_screen_id = 13;

    while (window.isOpen() && next_screen_id == 13) {
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            if (eventOpt->is<sf::Event::Closed>()) return SCREEN_EXIT;
            handleInput(*eventOpt, window);
        }

        update();
        render(window);
        window.display();
    }
    return next_screen_id;
}

void PackScreen::handleInput(const sf::Event& event, sf::RenderWindow&) {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = moved->position;
        btnBronze.setHover(btnBronze.isClicked(mousePos));
        btnGold.setHover(btnGold.isClicked(mousePos));
        btnBack.setHover(btnBack.isClicked(mousePos));
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (btnBack.isClicked(mousePos)) {
                this->next_screen_id = SCREEN_MAIN_MENU;
            }

            std::unique_ptr<Pachet> p = nullptr;
            if (btnBronze.isClicked(mousePos)) p = std::make_unique<PachetBronze>();
            else if (btnGold.isClicked(mousePos)) p = std::make_unique<PachetGold>();

            if (p) {
                if (echipaMea->get_buget() >= p->getCost()) {
                    echipaMea->adaugare_buget(-p->getCost());
                    ultimulCastigat = p->deschide(bazaJucatori);

                    if (ultimulCastigat) {
                        echipaMea->adaugaJucatorExistent(ultimulCastigat);
                        istoricRef.adauga(ultimulCastigat);
                        textCastig.setString("FELICITARI! L-ai primit pe:\n" + ultimulCastigat->get_nume() + " (OVR: " + std::to_string((int)ultimulCastigat->get_ovr()) + ")");
                    }
                } else {
                    textCastig.setString("Fonduri insuficiente!");
                    textCastig.setFillColor(sf::Color::Red);
                }
            }
        }
    }
}

void PackScreen::update() {
    textStatus.setString("Buget Manager: " + std::to_string(echipaMea->get_buget()) + " lei");
}

void PackScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 30, 60));

    btnBronze.draw(window);
    btnGold.draw(window);
    btnBack.draw(window);

    window.draw(textStatus);
    if (ultimulCastigat || textCastig.getString() != "") {
        window.draw(textCastig);
    }
}