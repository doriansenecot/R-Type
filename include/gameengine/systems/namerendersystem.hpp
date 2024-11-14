/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** namerendersystem.hpp
*/

#ifndef NAMERENDERSYSTEM_HPP_
#define NAMERENDERSYSTEM_HPP_

#include "components.hpp"
#include "system.hpp"
#include <SFML/Graphics.hpp>

class NameRenderSystem : public System {
  public:
    void render(sf::RenderWindow &window, sf::Font &font);
};

#endif // NAMERENDERSYSTEM_HPP_