#include "InitialDraftScreen.h"
#include "jucator.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <optional>

const float LISTA_X_START = 50.0f;
const float LISTA_Y_START = 100.0f;

InitialDraftScreen::InitialDraftScreen(Echipeptr echipa, BazaDeDateptr db, sf::Font& font)
    : echipaMea(echipa),
      baza(db),
      fontRef(font),
      bugetCurent(echipa->get_buget()),
      next_screen_id(SCREEN_DRAFT),
      finalizeButton(800.0f, 600.0f, 180.0f, 40.0f, "Finalizeaza Echipa (0/6)", font),
      mesajStatus(font),
      listaVizuala(font, baza->getLista(), LISTA_X_START, LISTA_Y_START),
      isTransitioning(false)
{
    mesajStatus.setCharacterSize(16);
    mesajStatus.setPosition(sf::Vector2f(50.0f, 650.0f));
    mesajStatus.setFillColor(sf::Color::White);
    mesajStatus.setString("Selecteaza 6 jucatori initiali. Buget: " + std::to_string(echipaMea->get_buget()) + " lei.");
}

int InitialDraftScreen::run(sf::RenderWindow& window) {
    this->next_screen_id = SCREEN_DRAFT;
    this->isTransitioning = false;
    this->selectieTemporara.clear();
    this->bugetCurent = echipaMea->get_buget();
    this->finalizeButton.setString("Finalizeaza Echipa (0/6)");
    this->mesajStatus.setString("Selecteaza 6 jucatori initiali. Buget: " + std::to_string(this->bugetCurent) + " lei.");
    this->mesajStatus.setFillColor(sf::Color::White);

    while (window.isOpen()) {
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                return SCREEN_EXIT;
            }

            if (!isTransitioning) {
                handleInput(event, window);
            }
        }

        if (isTransitioning) {
            if (transitionClock.getElapsedTime().asSeconds() >= 5.0f) {
                this->next_screen_id = SCREEN_MAIN_MENU;
                isTransitioning = false;
                break;
            }
        }
        else if (next_screen_id != SCREEN_DRAFT) {
            break;
        }

        update();
        render(window);
        window.display();
    }

    return next_screen_id;
}

void InitialDraftScreen::handleInput(const sf::Event& event, sf::RenderWindow& window) {
    if (const auto* moved = event.getIf<sf::Event::MouseMoved>()) {
        finalizeButton.setHover(finalizeButton.isClicked(moved->position));
    }

    if (const auto* released = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (released->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = released->position;

            if (finalizeButton.isClicked(mousePos)) {
                handleFinalizeButton();
            }
            else {
                handlePlayerClick(mousePos);
            }
        }
    }
}

void InitialDraftScreen::handlePlayerClick(sf::Vector2i mousePos) {
    int indexSelectat = listaVizuala.getIndexFromClick(mousePos);

    if (indexSelectat == -1) return;

    const auto& listaCompletaJucatori = baza->getLista();
    if (indexSelectat >= (int)listaCompletaJucatori.size()) return;

    jucatorptr jucatorAles = listaCompletaJucatori[indexSelectat];
    long long pretJucator = jucatorAles->get_pret();

    if (jucatorAles->get_echipe() != nullptr) {
        mesajStatus.setString("Jucatorul este deja contractat de o alta echipa!");
        mesajStatus.setFillColor(sf::Color(255, 140, 0));
        return;
    }

    auto it = std::find(selectieTemporara.begin(), selectieTemporara.end(), jucatorAles);

    if (it != selectieTemporara.end()) {
        selectieTemporara.erase(it);
        bugetCurent += pretJucator;
        mesajStatus.setString("Jucator deselectat. (Curent: " + std::to_string(selectieTemporara.size()) + "/6)");
        mesajStatus.setFillColor(sf::Color::White);
        finalizeButton.setString("Finalizeaza Echipa (" + std::to_string(selectieTemporara.size()) + "/6)");
        return;
    }

    if (selectieTemporara.size() >= 6) {
         mesajStatus.setString("Eroare: Ai selectat deja 6 jucatori.");
         mesajStatus.setFillColor(sf::Color::Red);
         return;
    }

    std::vector<jucatorptr> listaJucatoriDisponibili = baza->getLista();

    if (echipaMea->poate_achizitiona_si_finaliza(jucatorAles, listaJucatoriDisponibili)) {
         try {
             bugetCurent -= pretJucator;
             selectieTemporara.push_back(jucatorAles);
             mesajStatus.setString("Jucator adaugat! Buget ramas: " + std::to_string(bugetCurent) + " lei.");
             mesajStatus.setFillColor(sf::Color(0, 255, 0)); // Verde
             finalizeButton.setString("Finalizeaza Echipa (" + std::to_string(selectieTemporara.size()) + "/6)");
         } catch (const std::runtime_error& e) {
             mesajStatus.setString("EROARE INTERNĂ: " + std::string(e.what()));
             mesajStatus.setFillColor(sf::Color::Red);
         }
    } else {
         mesajStatus.setString("Eroare: Prea scump sau incalca structura echipei (min. buget necesar nesatisfacut).");
         mesajStatus.setFillColor(sf::Color::Red);
    }
}

void InitialDraftScreen::handleFinalizeButton() {
    if (selectieTemporara.size() != 6) {
        mesajStatus.setString("Eroare: Trebuie sa selectezi exact 6 jucatori!");
        mesajStatus.setFillColor(sf::Color::Red);
        return;
    }

    try {
        echipaMea->set_buget(echipaMea->get_buget());
        echipaMea->creare_in_gui(selectieTemporara);
        bugetCurent = echipaMea->get_buget();

        isTransitioning = true;
        transitionClock.restart();

        mesajStatus.setString("FELICITARI! Echipa a fost salvata. Trecere la Main Menu in 5 secunde...");
        mesajStatus.setFillColor(sf::Color::Green);


    } catch (const std::runtime_error& e) {
        isTransitioning = false;

        mesajStatus.setString("EROARE SALVARE: " + std::string(e.what()));
        mesajStatus.setFillColor(sf::Color::Red);

        for (const auto& j : selectieTemporara) {
            j->setAles(false);
        }
        selectieTemporara.clear();

        this->next_screen_id = SCREEN_DRAFT;
        finalizeButton.setString("Finalizeaza Echipa (0/6)");
    }
}

void InitialDraftScreen::update() { }

void InitialDraftScreen::render(sf::RenderWindow& window) {
    window.clear(sf::Color(30, 30, 40));
    sf::Text bugetText(fontRef);
    bugetText.setCharacterSize(20);
    bugetText.setPosition(sf::Vector2f(50.0f, 30.0f));
    bugetText.setString("Buget: " + std::to_string(bugetCurent) + " lei");
    window.draw(bugetText);

    listaVizuala.draw(window);

    finalizeButton.draw(window);
    window.draw(mesajStatus);
}