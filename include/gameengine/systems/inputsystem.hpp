/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** inputsystem.hpp
*/

#ifndef INPUTSYSTEM_HPP_
#define INPUTSYSTEM_HPP_

#include "components.hpp"
#include "coordinator.hpp"
#include "keymapping.hpp"
#include "system.hpp"

class InputSystem : public System {
  public:
    InputSystem() {}
    void setKeyMapping(KeyMapping &keyMapping) {
        this->keyMapping = &keyMapping;
    }
    void update();

  private:
    KeyMapping *keyMapping = nullptr;
};

#endif // INPUTSYSTEM_HPP_
