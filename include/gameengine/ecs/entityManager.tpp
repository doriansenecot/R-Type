/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** EntityManager.tpp
*/

#include "entitymanager.hpp"

template <typename ComponentType>
void EntityManager::addComponent(Entity entity, ComponentType component) {
    auto &componentMap = getComponentMap<ComponentType>();
    componentMap[entity] = component;
}

template <typename ComponentType>
void EntityManager::removeComponent(Entity entity) {
    auto &componentMap = getComponentMap<ComponentType>();
    componentMap.erase(entity);
}

template <typename ComponentType>
bool EntityManager::hasComponent(Entity entity) {
    auto &componentMap = getComponentMap<ComponentType>();
    return componentMap.find(entity) != componentMap.end();
}

template <typename ComponentType>
ComponentType &EntityManager::getComponent(Entity entity) {
    auto &componentMap = getComponentMap<ComponentType>();
    auto it = componentMap.find(entity);
    assert(it != componentMap.end());
    return it->second;
}

template <typename ComponentType>
std::unordered_map<Entity, ComponentType> &EntityManager::getComponentMap() {
    std::type_index typeIndex(typeid(ComponentType));
    if (componentStores.find(typeIndex) == componentStores.end()) {
        componentStores[typeIndex] =
            std::make_shared<std::unordered_map<Entity, ComponentType>>();
    }
    return *std::static_pointer_cast<std::unordered_map<Entity, ComponentType>>(
        componentStores[typeIndex]);
}
