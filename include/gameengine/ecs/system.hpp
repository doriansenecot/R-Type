/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** system.hpp
*/

#ifndef SYSTEM_HPP_
#define SYSTEM_HPP_

#include "entity.hpp"
#include <set>

class System {
  public:
    std::set<Entity> entities;
    virtual ~System() = default;
    virtual void onEntityDestroyed(Entity entity) {}
};

#endif // SYSTEM_HPP_
