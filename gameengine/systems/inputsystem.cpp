/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** inputsystem.cpp
*/

#include "inputsystem.hpp"
#include "coordinator.hpp"

extern Coordinator gCoordinator;

void InputSystem::update() {
    if (!keyMapping) {
        return;
    }
    for (auto const &entity : entities) {
        auto &input = gCoordinator.getComponent<InputComponent>(entity);
        input.up = sf::Keyboard::isKeyPressed(
            keyMapping->getKeyForAction(Action::MoveUp));
        input.down = sf::Keyboard::isKeyPressed(
            keyMapping->getKeyForAction(Action::MoveDown));
        input.left = sf::Keyboard::isKeyPressed(
            keyMapping->getKeyForAction(Action::MoveLeft));
        input.right = sf::Keyboard::isKeyPressed(
            keyMapping->getKeyForAction(Action::MoveRight));
        input.pressed = sf::Keyboard::isKeyPressed(
            keyMapping->getKeyForAction(Action::Fire));

        bool firePressed = input.pressed;
        if (firePressed && !input.spaceHeld) {
            input.spaceReleased = true;
            input.spaceHeld = true;
            input.isFiring = true;
        } else if (!firePressed) {
            input.spaceHeld = false;
            input.spaceReleased = false;
            input.isFiring = false;
        } else {
            input.spaceReleased = false;
        }
    }
}
