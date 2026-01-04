#pragma once
#include "Screen.h"
#include "Gestiune.h"
#include "Button.h"

template <typename T>
class HistoryScreen : public Screen {
private:
    Gestiune<T>& sursaDate;
    sf::Font& font;
    std::string titlu;
    Button backButton;


public:
    HistoryScreen(Gestiune<T>& sursa, sf::Font& f, std::string t) 
        : sursaDate(sursa), font(f), titlu(t),
          backButton(400.0f, 600.0f, 200.0f, 50.0f, "Inapoi", f) {}

    int run(sf::RenderWindow& window) override {
        this->next_screen_id = 10;
        
        while (window.isOpen()) {
            while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
                if (eventOpt->is<sf::Event::Closed>()) return SCREEN_EXIT;
                
                if (const auto* pressed = eventOpt->getIf<sf::Event::MouseButtonPressed>()) {
                    if (backButton.isClicked(pressed->position)) return SCREEN_MAIN_MENU;
                }
            }

            window.clear(sf::Color(30, 30, 60));
            
            sf::Text txtTitlu(font, titlu, 40);
            txtTitlu.setPosition({300, 20});
            window.draw(txtTitlu);

            auto elemente = sursaDate.getToate();
            for (size_t i = 0; i < elemente.size() && i < 12; ++i) {
                sf::Text itemText(font, "", 18);
                

                if constexpr (std::is_same_v<T, std::string>) {
                    itemText.setString(std::to_string(i+1) + ". " + elemente[i]);
                } else {
                    itemText.setString(std::to_string(i+1) + ". " + elemente[i]->to_string());
                }
                
                itemText.setPosition({100.0f, 100.0f + i * 35.0f});
                window.draw(itemText);
            }

            backButton.draw(window);
            window.display();
        }
        return SCREEN_MAIN_MENU;
    }

   // void handleInput(const sf::Event&, sf::RenderWindow&) override {}
  //  void update() override {}
   // void render(sf::RenderWindow&) override {}
};