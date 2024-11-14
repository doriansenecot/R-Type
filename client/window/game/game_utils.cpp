/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** game_utils
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

sf::IntRect Game::getTextureRectForClientID(int clientId) {
    sf::IntRect textureRect;
    switch (clientId % 5) {
    case 0:
        textureRect = sf::IntRect(66, 17, 33, 17);
        break;
    case 1:
        textureRect = sf::IntRect(0, 0, 33, 17);
        break;
    case 2:
        textureRect = sf::IntRect(0, 34, 33, 17);
        break;
    case 3:
        textureRect = sf::IntRect(0, 51, 33, 17);
        break;
    case 4:
        textureRect = sf::IntRect(0, 68, 33, 17);
        break;
    default:
        textureRect = sf::IntRect(0, 17, 33, 17);
        break;
    }
    return textureRect;
}

void Game::startFadeEffect(bool toBlack) {
    isFading = true;
    fadeIn = toBlack;
    fadeAlpha = toBlack ? 0.0f : 255.0f;
    fadeOverlay.setFillColor(
        sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));

    if (!toBlack) {
        isTextFading = true;
        textFadeAlpha = 0.0f;
        stageText.setFillColor(
            sf::Color(255, 255, 255, static_cast<sf::Uint8>(textFadeAlpha)));
    }
}

Entity Game::getEntityByClientId(int clientId) const {
    auto it = clientEntities.find(clientId);
    if (it != clientEntities.end()) {
        return it->second;
    }
    return -1;
}

void Game::checkPlayersInWaitingArea() {
    bool allInArea = true;
    for (const auto &[clientId, entity] : clientEntities) {
        if (entity == playerEntity)
            continue;

        if (!gCoordinator.hasComponent<TransformComponent>(entity)) {
            allInArea = false;
            break;
        }

        auto &transform = gCoordinator.getComponent<TransformComponent>(entity);
        sf::Vector2f pos = transform.position;

        sf::FloatRect areaBounds = waitingArea.getGlobalBounds();

        if (!areaBounds.contains(pos)) {
            allInArea = false;
            break;
        }
    }

    if (gCoordinator.hasComponent<TransformComponent>(playerEntity)) {
        auto &selfTransform =
            gCoordinator.getComponent<TransformComponent>(playerEntity);
        sf::Vector2f selfPos = selfTransform.position;
        sf::FloatRect areaBounds = waitingArea.getGlobalBounds();

        if (!areaBounds.contains(selfPos)) {
            allInArea = false;
        }
    } else {
        allInArea = false;
    }
    if (allInArea && isWaiting) {
        isWaiting = false;
        startFadeEffect(true);
        stageText.setString("Stage " +
                            std::to_string(stageSystem.getCurrentStage()));
        stageText.setOrigin(stageText.getLocalBounds().width / 2,
                            stageText.getLocalBounds().height / 2);
        showStageText = true;
        stageTextTimer = 0.0f;
        startFadeEffect(false);
    }
}
