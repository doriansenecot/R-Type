/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-dorian.senecot
** File description:
** minigame_menu.hpp
*/

#ifndef MINIGAME_MENU_HPP
#define MINIGAME_MENU_HPP

#include "client.hpp"
#include <SFML/Graphics.hpp>

class MiniGameMenu {
  public:
    MiniGameMenu(Client &client);
    void initialize();
    void update(float deltaTime);
    void render(sf::RenderWindow &window);
    void handleMouseClick();

  private:
    Client &_client;

    sf::Sprite _backgroundSprite;
    sf::Texture _backgroundTexture;

    sf::Sprite _backButtonSprite;
    sf::Texture _backButtonTexture;

    sf::Sprite _startButtonSprite;
    sf::Texture _startButtonTexture;

    size_t _cursorPosition;
    bool _isSelecting;
    bool _showCursor;
};

#endif // MINIGAME_MENU_HPP
