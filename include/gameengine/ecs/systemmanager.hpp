/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** systemmanager.hpp
*/

#ifndef SYSTEMMANAGER_HPP_
#define SYSTEMMANAGER_HPP_

#include "component.hpp"
#include "system.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>

class SystemManager {
  public:
    template <typename T> std::shared_ptr<T> registerSystem();

    template <typename T> void setSignature(Signature signature);

    void entityDestroyed(Entity entity);
    void entitySignatureChanged(Entity entity, Signature entitySignature);

  private:
    std::unordered_map<const char *, Signature> signatures{};
    std::unordered_map<const char *, std::shared_ptr<System>> systems{};
};

#include "systemmanager.tpp"

#endif // SYSTEMMANAGER_HPP_
