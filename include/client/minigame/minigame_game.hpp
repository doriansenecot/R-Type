/*
** EPITECH PROJECT, 2024
** RType
** File description:
** minigame_game
*/

#ifndef MINIGAME_GAME_HPP
#define MINIGAME_GAME_HPP

#include "client.hpp"
#include "coordinator.hpp"
#include "systems/movementsystem.hpp"
#include "systems/rendersystem.hpp"
#include <SFML/Graphics.hpp>

class MiniGame {
  public:
    MiniGame(Client &client);
    void initialize();
    void render(sf::RenderWindow &window);
    void update(float deltaTime);
    void handleMouseClick();

  private:
    Client &_client;
    sf::Sprite _backgroundSprite;
    sf::Texture _backgroundTexture;
    sf::Vector2u _backgroundSize;

    sf::Sprite _playerSprite;
    sf::Texture _playerTexture;
    Entity playerEntity;

    std::shared_ptr<MovementSystem> movementSystem;
    std::shared_ptr<RenderSystem> renderSystem;

    void setupCharacter(Entity entity, sf::Vector2f position,
                        sf::IntRect textureRect);
};

#endif // MINIGAME_GAME_HPP
