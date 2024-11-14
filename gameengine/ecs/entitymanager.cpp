/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** entitymanager.cpp
*/

#include "entitymanager.hpp"
#include <iostream>

EntityManager::EntityManager() : nextEntityId(0), livingEntityCount(0) {
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        availableEntities.push(entity);
    }
}

Entity EntityManager::createEntity() {
    if (livingEntityCount >= MAX_ENTITIES) {
        throw std::runtime_error(
            "Maximum entity limit reached, cannot create more entities.");
    }

    Entity id = availableEntities.front();
    availableEntities.pop();
    ++livingEntityCount;
    return id;
}

void EntityManager::destroyEntity(Entity entity) {
    if (entity >= MAX_ENTITIES) {
        std::cerr << "Warning: Attempt to destroy entity out of range: "
                  << entity << std::endl;
        return;
    }

    signatures[entity].reset();
    availableEntities.push(entity);
    --livingEntityCount;
}

void EntityManager::setSignature(Entity entity, Signature signature) {
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    signatures[entity] = signature;
}

Signature EntityManager::getSignature(Entity entity) {
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    return signatures[entity];
}

Entity EntityManager::create_entity() { return nextEntityId++; }

void EntityManager::destroy_entity(Entity entity) {
    entities.erase(std::remove(entities.begin(), entities.end(), entity),
                   entities.end());

    for (auto &[type, store] : componentStores) {
        if (type == std::type_index(typeid(PositionComponent))) {
            auto componentMap = std::static_pointer_cast<
                std::unordered_map<Entity, PositionComponent>>(store);
            componentMap->erase(entity);
        } else if (type == std::type_index(typeid(VelocityComponent))) {
            auto componentMap = std::static_pointer_cast<
                std::unordered_map<Entity, VelocityComponent>>(store);
            componentMap->erase(entity);
        } else if (type == std::type_index(typeid(TypeComponent))) {
            auto componentMap = std::static_pointer_cast<
                std::unordered_map<Entity, TypeComponent>>(store);
            componentMap->erase(entity);
        } else if (type == std::type_index(typeid(OwnerComponent))) {
            auto componentMap = std::static_pointer_cast<
                std::unordered_map<Entity, OwnerComponent>>(store);
            componentMap->erase(entity);
        }
    }
}
