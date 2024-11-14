/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** movementsystem.cpp
*/

#include "movementsystem.hpp"
#include "coordinator.hpp"
#include <iostream>

extern Coordinator gCoordinator;

void MovementSystem::update(float deltaTime) {
    for (auto it = entities.begin(); it != entities.end();) {
        Entity entity = *it;

        if (gCoordinator.hasComponent<VelocityComponent>(entity) &&
            gCoordinator.hasComponent<TransformComponent>(entity)) {

            auto &velocity =
                gCoordinator.getComponent<VelocityComponent>(entity);
            auto &transform =
                gCoordinator.getComponent<TransformComponent>(entity);

            transform.position += velocity.velocity * deltaTime;

            ++it;
        } else {
            std::cerr << "Warning: Entity " << entity
                      << " is missing required components. Removing from "
                         "MovementSystem."
                      << std::endl;
            it = entities.erase(it);
        }
    }
}

void MovementSystem::update(EntityManager &entityManager, float deltaTime) {
    auto &positions = entityManager.getComponentMap<PositionComponent>();
    auto &velocities = entityManager.getComponentMap<VelocityComponent>();

    for (auto &[entity, position] : positions) {
        if (velocities.find(entity) != velocities.end()) {
            auto &velocity = velocities[entity];
            position.x += velocity.velocity.x * deltaTime;
            position.y += velocity.velocity.y * deltaTime;
        }
    }
}
