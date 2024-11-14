/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** game_setup
*/

#include "client.hpp"
#include "components.hpp"
#include "coordinator.hpp"
#include "game.hpp"
#include "inputsystem.hpp"
#include "movementsystem.hpp"
#include "pathhelper.hpp"
#include "rendersystem.hpp"
#include "scenemanager.hpp"
#include "shadersystem.hpp"
#include "utils.hpp"
#include <cmath>
#include <iostream>
#include <map>

void Game::setupLoadingCircle() {
    float radius = 50.0f;
    int pointCount = 60;
    float startAngle = 30.0f;
    float endAngle = 330.0f;

    loadingCircle.setPrimitiveType(sf::TriangleFan);
    loadingCircle.clear();
    sf::Vector2f center(
        waitingArea.getPosition().x + waitingArea.getSize().x / 2,
        waitingArea.getPosition().y + waitingArea.getSize().y / 2);
    loadingCircle.append(sf::Vertex(center, sf::Color::Red));
    for (int i = 0; i <= pointCount; ++i) {
        float angle = startAngle + (i * (endAngle - startAngle) / pointCount);
        float radian = angle * M_PI / 180.0f;
        float x = center.x + radius * cos(radian);
        float y = center.y + radius * sin(radian);
        loadingCircle.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Black));
    }
}

void Game::setupWaitingArea() {
    float areaWidth = 400.0f;
    float areaHeight = 400.0f;
    float posX = (1920 - areaWidth) / 2.0f;
    float posY = (1080 - areaHeight) / 2.0f;

    waitingArea.setPosition(posX, posY);
    waitingArea.setSize(sf::Vector2f(areaWidth, areaHeight));
    waitingArea.setFillColor(sf::Color::Transparent);
    waitingArea.setOutlineThickness(5.0f);
    waitingArea.setOutlineColor(sf::Color::Red);
}

void Game::setupSpaceship(Entity entity, sf::Vector2f position,
                          sf::IntRect textureRect, const std::string &name) {
    TransformComponent transform;
    transform.position = position;
    transform.scale = sf::Vector2f(2.5f, 2.5f);
    transform.rotation = 0.0f;
    gCoordinator.addComponent(entity, transform);

    SpriteComponent sprite;
    sprite.sprite.setTexture(spaceshipTexture);
    sprite.sprite.setTextureRect(textureRect);
    gCoordinator.addComponent(entity, sprite);

    VelocityComponent velocity;
    velocity.velocity = sf::Vector2f(0.0f, 0.0f);
    gCoordinator.addComponent(entity, velocity);

    InputComponent input;
    gCoordinator.addComponent(entity, input);

    NameComponent nameComp;
    nameComp.name = name;
    gCoordinator.addComponent(entity, nameComp);

    HealthComponent health;
    health.currentHealth = 100;
    health.maxHealth = 100;
    gCoordinator.addComponent(entity, health);

    XPComponent xp;
    xp.currentXP = 0;
    gCoordinator.addComponent(entity, xp);

    LevelComponent level;
    level.currentLevel = 1;
    gCoordinator.addComponent(entity, level);

    std::cout << "Spaceship created at position (" << position.x << ", "
              << position.y << ") with health " << std::endl;
}
