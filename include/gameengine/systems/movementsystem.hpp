/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** movementsystem.hpp
*/

#ifndef MOVEMENTSYSTEM_HPP_
#define MOVEMENTSYSTEM_HPP_

#include "components.hpp"
#include "coordinator.hpp"
#include "entityManager.tpp"
#include "system.hpp"

class MovementSystem : public System {
  public:
    void update(float deltaTime);
    void update(EntityManager &entityManager, float deltaTime);
};

#endif // MOVEMENTSYSTEM_HPP_
