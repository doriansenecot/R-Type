/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** coordinator.tpp
*/

#include "coordinator.hpp"

template <typename T> void Coordinator::registerComponent() {
    componentManager->registerComponent<T>();
}

template <typename T>
void Coordinator::addComponent(Entity entity, T component) {
    componentManager->addComponent<T>(entity, component);

    auto signature = entityManager->getSignature(entity);
    signature.set(componentManager->getComponentType<T>(), true);
    entityManager->setSignature(entity, signature);

    systemManager->entitySignatureChanged(entity, signature);
}

template <typename T> void Coordinator::removeComponent(Entity entity) {
    componentManager->removeComponent<T>(entity);

    auto signature = entityManager->getSignature(entity);
    signature.set(componentManager->getComponentType<T>(), false);
    entityManager->setSignature(entity, signature);

    systemManager->entitySignatureChanged(entity, signature);
}

template <typename T> T &Coordinator::getComponent(Entity entity) {
    return componentManager->getComponent<T>(entity);
}

template <typename T> ComponentType Coordinator::getComponentType() {
    return componentManager->getComponentType<T>();
}

template <typename T> std::shared_ptr<T> Coordinator::registerSystem() {
    return systemManager->registerSystem<T>();
}

template <typename T>
void Coordinator::setSystemSignature(Signature signature) {
    systemManager->setSignature<T>(signature);
}

template <typename T> bool Coordinator::hasComponent(Entity entity) {
    return componentManager->hasComponent<T>(entity);
}