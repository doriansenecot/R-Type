/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** coordinator.hpp
*/

#ifndef COORDINATOR_HPP_
#define COORDINATOR_HPP_

#include "componentmanager.hpp"
#include "entitymanager.hpp"
#include "systemmanager.hpp"
#include <memory>

class Coordinator {
  public:
    void init();

    Entity createEntity();
    void destroyEntity(Entity entity);

    template <typename T> void registerComponent();

    template <typename T> void addComponent(Entity entity, T component);

    template <typename T> void removeComponent(Entity entity);

    template <typename T> T &getComponent(Entity entity);

    template <typename T> ComponentType getComponentType();

    template <typename T> std::shared_ptr<T> registerSystem();

    template <typename T> void setSystemSignature(Signature signature);

    template <typename T> bool hasComponent(Entity entity);

  private:
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<SystemManager> systemManager;
};
#include "coordinator.tpp"

#endif // COORDINATOR_HPP_
