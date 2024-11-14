/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** componentmanager.tpp
*/

#include "componentmanager.hpp"

template <typename T>
void ComponentArray<T>::insertData(Entity entity, T component) {
    assert(entityToIndexMap.find(entity) == entityToIndexMap.end() &&
           "Component added to same entity more than once.");

    std::size_t newIndex = size;
    entityToIndexMap[entity] = newIndex;
    indexToEntityMap[newIndex] = entity;
    componentArray[newIndex] = component;
    ++size;
}

template <typename T> void ComponentArray<T>::removeData(Entity entity) {
    assert(entityToIndexMap.find(entity) != entityToIndexMap.end() &&
           "Removing non-existent component.");

    std::size_t indexOfRemovedEntity = entityToIndexMap[entity];
    std::size_t indexOfLastElement = size - 1;
    componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

    Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
    entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
    indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

    entityToIndexMap.erase(entity);
    indexToEntityMap.erase(indexOfLastElement);

    --size;
}

template <typename T> T &ComponentArray<T>::getData(Entity entity) {
    assert(entityToIndexMap.find(entity) != entityToIndexMap.end() &&
           "Retrieving non-existent component.");
    return componentArray[entityToIndexMap[entity]];
}

template <typename T> void ComponentArray<T>::entityDestroyed(Entity entity) {
    if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
        removeData(entity);
    }
}

template <typename T> void ComponentManager::registerComponent() {
    const char *typeName = typeid(T).name();
    assert(componentTypes.find(typeName) == componentTypes.end() &&
           "Registering component type more than once.");

    componentTypes.insert({typeName, nextComponentType});
    componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
    ++nextComponentType;
}

template <typename T> ComponentType ComponentManager::getComponentType() {
    const char *typeName = typeid(T).name();
    assert(componentTypes.find(typeName) != componentTypes.end() &&
           "Component not registered before use.");

    return componentTypes[typeName];
}

template <typename T>
void ComponentManager::addComponent(Entity entity, T component) {
    getComponentArray<T>()->insertData(entity, component);
}

template <typename T> void ComponentManager::removeComponent(Entity entity) {
    getComponentArray<T>()->removeData(entity);
}

template <typename T> T &ComponentManager::getComponent(Entity entity) {
    return getComponentArray<T>()->getData(entity);
}

template <typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::getComponentArray() {
    const char *typeName = typeid(T).name();
    assert(componentTypes.find(typeName) != componentTypes.end() &&
           "Component not registered before use.");
    auto componentArray =
        std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
    assert(componentArray && "Component array not initialized.");
    return componentArray;
}

template <typename T> bool ComponentArray<T>::hasComponent(Entity entity) {
    return entityToIndexMap.find(entity) != entityToIndexMap.end();
}

template <typename T> bool ComponentManager::hasComponent(Entity entity) {
    return getComponentArray<T>()->hasComponent(entity);
}
