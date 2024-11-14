/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** rendersystem.cpp
*/

#include "rendersystem.hpp"
#include "coordinator.hpp"
#include <iostream>

extern Coordinator gCoordinator;

void RenderSystem::render(sf::RenderWindow &window) {
    for (auto it = entities.begin(); it != entities.end();) {
        Entity entity = *it;

        if (gCoordinator.hasComponent<SpriteComponent>(entity) &&
            gCoordinator.hasComponent<TransformComponent>(entity)) {

            auto &sprite = gCoordinator.getComponent<SpriteComponent>(entity);
            auto &transform =
                gCoordinator.getComponent<TransformComponent>(entity);

            sprite.sprite.setPosition(transform.position);
            sprite.sprite.setRotation(transform.rotation);
            sprite.sprite.setScale(transform.scale);

            window.draw(sprite.sprite);

            ++it;
        } else {
            std::cerr << "Warning: Entity " << entity
                      << " does not have the required components. Removing "
                         "from RenderSystem."
                      << std::endl;
            it = entities.erase(it);
        }
    }
}
