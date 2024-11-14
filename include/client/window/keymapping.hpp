/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-dorian.senecot
** File description:
** keymapping.hpp
*/

#ifndef KEYMAPPING_HPP_
#define KEYMAPPING_HPP_

#include <SFML/Window/Keyboard.hpp>
#include <map>

enum class Action {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Fire,
};

class KeyMapping {
  public:
    KeyMapping();
    sf::Keyboard::Key getKeyForAction(Action action) const;
    void setKeyForAction(Action action, sf::Keyboard::Key key);

  private:
    std::map<Action, sf::Keyboard::Key> keyMap;
};

#endif // KEYMAPPING_HPP_
