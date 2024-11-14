/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-dorian.senecot
** File description:
** explosionsystem.cpp
*/

#include "explosionsystem.hpp"

void ExplosionSystem::update(float deltaTime) {
    std::vector<Entity> entitiesToDestroy;

    for (auto const &entity : entities) {
        auto &animation = gCoordinator.getComponent<AnimationComponent>(entity);
        auto &sprite = gCoordinator.getComponent<SpriteComponent>(entity);

        if (animation.monsterType == 1000) {
            animation.timer += deltaTime;
            if (animation.timer >= animation.frameTime) {
                animation.frameIndex++;
                animation.timer = 0.0f;
                if (animation.frameIndex >= animation.numFrames) {
                    entitiesToDestroy.push_back(entity);
                    continue;
                }

                int xOffset = animation.frameIndex * 66;
                int yOffset = 98;
                sprite.sprite.setTextureRect(
                    sf::IntRect(xOffset, yOffset, 66, 65));
            }
        }
    }
    for (const auto &entity : entitiesToDestroy) {
        gCoordinator.destroyEntity(entity);
    }
}
