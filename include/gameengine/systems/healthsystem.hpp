/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** healthsystem.hpp
*/

#ifndef HEALTHSYSTEM_HPP_
#define HEALTHSYSTEM_HPP_

#include "components.hpp"
#include "system.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>

class HealthSystem : public System {
  public:
    void render(sf::RenderWindow &window, Entity playerEntity);

  private:
    void drawHealthBar(sf::RenderWindow &window,
                       const TransformComponent &transform,
                       const HealthComponent &health);
};

#endif // HEALTHSYSTEM_HPP_
