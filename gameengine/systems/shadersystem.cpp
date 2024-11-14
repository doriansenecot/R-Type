/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** ShaderSystem.cpp
*/

#include "shadersystem.hpp"
#include "components.hpp"
#include "coordinator.hpp"
#include "pathhelper.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

extern Coordinator gCoordinator;

void ShaderSystem::applyShaders(sf::RenderWindow &window,
                                sf::Clock &globalClock) {
    float globalTime = globalClock.getElapsedTime().asSeconds();

    for (auto const &entity : entities) {
        auto &spriteComp = gCoordinator.getComponent<SpriteComponent>(entity);
        auto &shaderComp = gCoordinator.getComponent<ShaderComponent>(entity);

        if (shaderComp.useShader) {
            shaderComp.shader->setUniform("time", globalTime);
            sf::RenderStates states;
            states.shader = shaderComp.shader.get();
            window.draw(spriteComp.sprite, states);
        } else {
            window.draw(spriteComp.sprite);
        }
    }
}

void ShaderSystem::setRainbowShader(Entity entity, float time) {
    auto &shaderComp = gCoordinator.getComponent<ShaderComponent>(entity);
    if (shaderComp.useShader) {
        shaderComp.shader->setUniform("time", time);
    }
}

bool ShaderSystem::loadRainbowShader(sf::Shader &shader) {
    PathHelper pathHelper;
    std::string shaderPath = pathHelper.getShaderPath("rainbow.frag");
    if (!shader.loadFromFile(shaderPath, sf::Shader::Fragment)) {
        std::cerr << "Failed to load shader: " << shaderPath << std::endl;
        return false;
    }
    return true;
}
