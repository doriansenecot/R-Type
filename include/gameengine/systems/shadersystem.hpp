/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** ShaderSystem.hpp
*/

#ifndef SHADERSYSTEM_HPP_
#define SHADERSYSTEM_HPP_

#include "components.hpp"
#include "coordinator.hpp"
#include "system.hpp"
#include <SFML/Graphics.hpp>

class ShaderSystem : public System {
  public:
    void applyShaders(sf::RenderWindow &window, sf::Clock &globalClock);
    void setRainbowShader(Entity entity, float time);
    bool loadRainbowShader(sf::Shader &shader);
};

#endif // SHADERSYSTEM_HPP_
