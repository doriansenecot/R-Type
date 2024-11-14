/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** systemmanager.tpp
*/

#include "systemmanager.hpp"

template <typename T> std::shared_ptr<T> SystemManager::registerSystem() {
    const char *typeName = typeid(T).name();

    assert(systems.find(typeName) == systems.end() &&
           "Registering system more than once.");

    auto system = std::make_shared<T>();
    systems.insert({typeName, system});
    return system;
}

template <typename T> void SystemManager::setSignature(Signature signature) {
    const char *typeName = typeid(T).name();

    assert(systems.find(typeName) != systems.end() &&
           "System used before registered.");

    signatures.insert({typeName, signature});
}
