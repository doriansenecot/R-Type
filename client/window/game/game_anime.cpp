/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** game_anime
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

void Game::animateMonster(Entity entity, float deltaTime) {
    auto &animation = gCoordinator.getComponent<AnimationComponent>(entity);
    auto &sprite = gCoordinator.getComponent<SpriteComponent>(entity);

    animation.timer += deltaTime;
    if (animation.timer >= animation.frameTime) {
        animation.frameIndex = (animation.frameIndex + 1) % animation.numFrames;

        int xOffset = 0;
        int yOffset = 0;

        switch (animation.monsterType) {
        case 0:
            xOffset = animation.frameIndex * 33;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 0, 33, 36));
            break;
        case 1:
            xOffset = animation.frameIndex * 65;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 0, 65, 50));
            break;
        case 2:
            xOffset = (animation.frameIndex % 8) * 33;
            yOffset = (animation.frameIndex / 8) * 34;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, yOffset, 33, 34));
            break;
        case 3:
            xOffset = (animation.frameIndex % 3) * 33;
            yOffset = (animation.frameIndex / 3) * 33;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, yOffset, 33, 33));
            break;
        case 4:
            xOffset = animation.frameIndex * 33;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, yOffset, 33, 33));
            break;
        case 5:
            xOffset = 271 + animation.frameIndex * 17;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 8, 17, 16));
            break;
        case 6:
            xOffset = animation.frameIndex * 33;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 66, 33, 33));
            break;
        case 7:
            xOffset = animation.frameIndex * 50;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 0, 50, 50));
            break;
        case 8:
            xOffset = (animation.frameIndex % 8) * 33;
            yOffset = (animation.frameIndex / 8) * 33;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, yOffset, 33, 33));
            break;
        case 10:
            xOffset = animation.frameIndex * 65;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 0, 65, 132));
            break;
        case 20: {
            int frameWidth = 264;
            int frameHeight = 143;
            int numFramesPerRow = 2;
            int numRows = 4;
            int frameIndex = animation.frameIndex % (numFramesPerRow * numRows);
            bool reverseRow = (frameIndex / numFramesPerRow) % 2 == 1;
            xOffset =
                reverseRow
                    ? (numFramesPerRow - 1 - (frameIndex % numFramesPerRow)) *
                          frameWidth
                    : (frameIndex % numFramesPerRow) * frameWidth;
            yOffset = (frameIndex / numFramesPerRow) * frameHeight;
            sprite.sprite.setTextureRect(
                sf::IntRect(xOffset, yOffset, frameWidth, frameHeight));
            break;
        }
        case 30:
            xOffset = animation.frameIndex * 160;
            sprite.sprite.setTextureRect(
                sf::IntRect(xOffset + 42, 278, 100, 115));
            break;
        case 31:
            xOffset = animation.frameIndex * 34;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 0, 34, 34));
            break;
        case 40:
            xOffset = animation.frameIndex * 64;
            sprite.sprite.setTextureRect(sf::IntRect(xOffset, 0, 64, 64));
            break;
        default:
            break;
        }

        animation.timer = 0.0f;
    }
}

void Game::animatePowerUp(Entity entity, float deltaTime) {
    auto &animation = gCoordinator.getComponent<AnimationComponent>(entity);
    auto &sprite = gCoordinator.getComponent<SpriteComponent>(entity);
    float speedMultiplier = 1.5f;
    animation.timer += deltaTime * speedMultiplier;

    if (animation.timer >= animation.frameTime) {
        animation.frameIndex = (animation.frameIndex + 1) % animation.numFrames;
        int xOffset = animation.frameIndex * 17;
        sprite.sprite.setTextureRect(sf::IntRect(xOffset, 0, 17, 18));
        animation.timer = 0.0f;
    }
}

void Game::setAnimationFrame(Entity entity, sf::Vector2i direction) {
    auto &sprite = gCoordinator.getComponent<SpriteComponent>(entity);
    int yOffset = sprite.sprite.getTextureRect().top;

    if (direction.y > 0) {
        sprite.sprite.setTextureRect(sf::IntRect(0, yOffset, 33, 17));
    } else if (direction.y < 0) {
        sprite.sprite.setTextureRect(sf::IntRect(99, yOffset, 33, 17));
    } else {
        sprite.sprite.setTextureRect(sf::IntRect(66, yOffset, 33, 17));
    }
}
