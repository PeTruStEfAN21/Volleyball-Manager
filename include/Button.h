#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;

    sf::Color baseColor;

public:
    Button(float x, float y, float width, float height, const std::string& label, sf::Font& fontRef);

    void draw(sf::RenderWindow& window);

    bool isClicked(sf::Vector2i mousePos) const;

    void setString(const std::string& newText);

    void setHover(bool isHovering);

    void setBaseColor(const sf::Color& color);

    sf::Vector2f getPosition() const;

    void setPosition(const sf::Vector2f& pos);
};