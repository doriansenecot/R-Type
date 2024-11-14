/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** componentmanager.cpp
*/

#include "componentmanager.hpp"

void ComponentManager::entityDestroyed(Entity entity) {
    for (auto const &pair : componentArrays) {
        auto const &componentArray = pair.second;
        componentArray->entityDestroyed(entity);
    }
}
