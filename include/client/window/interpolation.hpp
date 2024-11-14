/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** interpolation.hpp
*/

#ifndef INTERPOLATION_HPP_
#define INTERPOLATION_HPP_
#include <SFML/Graphics.hpp>

struct InterpolationData {
    sf::Vector2f startPosition;
    sf::Vector2f targetPosition;
    float elapsedTime;
    float totalTime;

    InterpolationData()
        : startPosition(0.0f, 0.0f), targetPosition(0.0f, 0.0f),
          elapsedTime(0.0f), totalTime(0.05f) {}
};

#endif // INTERPOLATION_HPP_