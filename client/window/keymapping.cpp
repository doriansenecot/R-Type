/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-dorian.senecot
** File description:
** keymapping.cpp
*/

#include "keymapping.hpp"

KeyMapping::KeyMapping() {
    keyMap[Action::MoveUp] = sf::Keyboard::Up;
    keyMap[Action::MoveDown] = sf::Keyboard::Down;
    keyMap[Action::MoveLeft] = sf::Keyboard::Left;
    keyMap[Action::MoveRight] = sf::Keyboard::Right;
    keyMap[Action::Fire] = sf::Keyboard::Space;
}

sf::Keyboard::Key KeyMapping::getKeyForAction(Action action) const {
    return keyMap.at(action);
}

void KeyMapping::setKeyForAction(Action action, sf::Keyboard::Key key) {
    keyMap[action] = key;
}
