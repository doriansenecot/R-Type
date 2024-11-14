/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** utils
*/

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

bool initTexture(sf::Sprite &sprite, sf::Texture &texture,
                 const std::string &filePath, const sf::Vector2f &position,
                 const sf::Vector2f &scale);
bool initText(sf::Text &text, const std::string &content,
              const sf::Vector2f &position, const sf::Color &color,
              unsigned int size);
bool initButtonFrame(sf::RectangleShape &frame, const sf::Vector2f &size,
                     const sf::Vector2f &position, const sf::Color &fillColor,
                     const sf::Color &outlineColor, float outlineThickness);

#endif /* !UTILS_HPP_ */
