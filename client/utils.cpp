/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** utils
*/

#include "utils.hpp"
#include "pathhelper.hpp"

bool initTexture(sf::Sprite &sprite, sf::Texture &texture,
                 const std::string &fileName, const sf::Vector2f &position,
                 const sf::Vector2f &scale) {
    PathHelper pathHelper;
    std::string filePath = pathHelper.getImagePath(fileName);

    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading sprite texture: " << filePath << std::endl;
        return false;
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(scale);
    return true;
}

bool initText(sf::Text &text, const std::string &content,
              const sf::Vector2f &position, const sf::Color &color,
              unsigned int size) {
    text.setString(content);
    text.setPosition(position);
    text.setFillColor(color);
    text.setCharacterSize(size);
    return true;
}

bool initButtonFrame(sf::RectangleShape &frame, const sf::Vector2f &size,
                     const sf::Vector2f &position, const sf::Color &fillColor,
                     const sf::Color &outlineColor, float outlineThickness) {
    frame.setSize(size);
    frame.setPosition(position);
    frame.setFillColor(fillColor);
    frame.setOutlineThickness(outlineThickness);
    frame.setOutlineColor(outlineColor);
    return true;
}
