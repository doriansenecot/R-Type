/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** coordinator.cpp
*/

#include "coordinator.hpp"
#include "componentmanager.hpp"

void Coordinator::init() {
    entityManager = std::make_unique<EntityManager>();
    componentManager = std::make_unique<ComponentManager>();
    systemManager = std::make_unique<SystemManager>();
}

Entity Coordinator::createEntity() { return entityManager->createEntity(); }

void Coordinator::destroyEntity(Entity entity) {
    entityManager->destroyEntity(entity);
    componentManager->entityDestroyed(entity);
    systemManager->entityDestroyed(entity);
}
