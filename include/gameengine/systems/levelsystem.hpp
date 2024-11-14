/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** levelsystem.hpp
*/

#ifndef LEVELYSTEM_HPP_
#define LEVELYSTEM_HPP_

#include "components.hpp"
#include "system.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>

class LevelSystem : public System {
  public:
    void render(sf::RenderWindow &window, Entity playerEntity, sf::Font &font);
};

#endif // HEALTHSYSTEM_HPP_
