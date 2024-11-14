/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** rendersystem.hpp
*/

#ifndef RENDERSYSTEM_HPP_
#define RENDERSYSTEM_HPP_

#include "components.hpp"
#include "coordinator.hpp"
#include "system.hpp"

class RenderSystem : public System {
  public:
    void render(sf::RenderWindow &window);
};

#endif // RENDERSYSTEM_HPP_
