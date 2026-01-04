#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& label, sf::Font& fontRef):text(fontRef) {

    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(sf::Vector2f(x, y));

    baseColor = sf::Color(100, 100, 100); 
    shape.setFillColor(baseColor); 
    

    text.setString(label);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);

    
    sf::FloatRect textRect = text.getLocalBounds();
    float originX = textRect.position.x + textRect.size.x / 2.0f;
    float originY = textRect.position.y + textRect.size.y / 2.0f;

    text.setOrigin(sf::Vector2f(originX,originY));

    text.setPosition(sf::Vector2f(x + width / 2.0f, y + height / 2.0f));
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}


void Button::setString(const std::string& newText) {
    text.setString(newText);

    sf::FloatRect textRect = text.getLocalBounds();
    float originX = textRect.position.x + textRect.size.x / 2.0f;
    float originY = textRect.position.y + textRect.size.y / 2.0f;

    text.setOrigin(sf::Vector2f(originX, originY));

    float x = shape.getPosition().x;
    float y = shape.getPosition().y;
    float width = shape.getSize().x;
    float height = shape.getSize().y;

    text.setPosition(sf::Vector2f(x + width / 2.0f, y + height / 2.0f));
}

bool Button::isClicked(sf::Vector2i mousePos) const {

    return shape.getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)));
}


void Button::setHover(bool isHovering) {
    if (isHovering) {
        shape.setFillColor(sf::Color(150, 150, 150));
    } else {
        shape.setFillColor(baseColor);
    }
}

void Button::setBaseColor(const sf::Color& color) {
    baseColor = color;
    shape.setFillColor(color);
}

sf::Vector2f Button::getPosition() const {
    return shape.getPosition();
}


void Button::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);


    float width = shape.getSize().x;
    float height = shape.getSize().y;

    sf::FloatRect textRect = text.getLocalBounds();
    float originX = textRect.position.x + textRect.size.x / 2.0f;
    float originY = textRect.position.y + textRect.size.y / 2.0f;

    text.setOrigin(sf::Vector2f(originX, originY));

    text.setPosition(sf::Vector2f(pos.x + width / 2.0f, pos.y + height / 2.0f));
}