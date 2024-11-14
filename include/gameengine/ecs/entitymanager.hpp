/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** entitymanager.hpp
*/

#ifndef ENTITYMANAGER_HPP_
#define ENTITYMANAGER_HPP_

#include "component.hpp"
#include "componentmanager.hpp"
#include "components.hpp"
#include "entity.hpp"
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <memory>
#include <queue>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

class EntityManager {
  public:
    EntityManager();

    Entity createEntity();
    void destroyEntity(Entity entity);
    Entity create_entity();
    void destroy_entity(Entity entity);

    void setSignature(Entity entity, Signature signature);
    Signature getSignature(Entity entity);
    const std::vector<Entity> &getEntities() const { return entities; }
    int getEntityCount() const { return entities.size(); }
    template <typename ComponentType>
    void addComponent(Entity entity, ComponentType component);

    template <typename ComponentType> void removeComponent(Entity entity);

    template <typename ComponentType> bool hasComponent(Entity entity);

    template <typename ComponentType>
    ComponentType &getComponent(Entity entity);

    template <typename ComponentType>
    std::unordered_map<Entity, ComponentType> &getComponentMap();
    std::queue<Entity> availableEntities;
    int livingEntityCount;

  private:
    std::array<Signature, MAX_ENTITIES> signatures{};
    Entity nextEntityId;
    std::vector<Entity> entities;
    std::unordered_map<std::type_index, std::shared_ptr<void>> componentStores;
};

#endif // ENTITYMANAGER_HPP_
