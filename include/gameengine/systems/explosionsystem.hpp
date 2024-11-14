/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** explosionsystem
*/

#ifndef EXPLOSIONSYSTEM_HPP_
#define EXPLOSIONSYSTEM_HPP_

#include "components.hpp"
#include "coordinator.hpp"
#include "system.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>

extern Coordinator gCoordinator;

class ExplosionSystem : public System {
  public:
    void update(float deltaTime);
};

#endif /* !EXPLOSIONSYSTEM_HPP_ */
