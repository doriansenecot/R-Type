/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** componentmanager.hpp
*/

#ifndef COMPONENTMANAGER_HPP_
#define COMPONENTMANAGER_HPP_

#include "component.hpp"
#include "entity.hpp"
#include <array>
#include <cassert>
#include <memory>
#include <unordered_map>

class IComponentArray {
  public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

template <typename T> class ComponentArray : public IComponentArray {
  public:
    void insertData(Entity entity, T component);
    void removeData(Entity entity);
    T &getData(Entity entity);
    void entityDestroyed(Entity entity) override;
    bool hasComponent(Entity entity);

  private:
    std::array<T, MAX_ENTITIES> componentArray;
    std::unordered_map<Entity, std::size_t> entityToIndexMap;
    std::unordered_map<std::size_t, Entity> indexToEntityMap;
    std::size_t size{};
};

class ComponentManager {
  public:
    template <typename T> void registerComponent();

    template <typename T> ComponentType getComponentType();

    template <typename T> void addComponent(Entity entity, T component);

    template <typename T> void removeComponent(Entity entity);

    template <typename T> T &getComponent(Entity entity);

    template <typename T> bool hasComponent(Entity entity);

    void entityDestroyed(Entity entity);

  private:
    std::unordered_map<const char *, ComponentType> componentTypes{};
    std::unordered_map<const char *, std::shared_ptr<IComponentArray>>
        componentArrays{};
    ComponentType nextComponentType{};

    template <typename T>
    std::shared_ptr<ComponentArray<T>> getComponentArray();
};
#include "componentmanager.tpp"

#endif // COMPONENTMANAGER_HPP_
