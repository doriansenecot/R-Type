/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** game.cpp
*/

#include "game.hpp"
#include "client.hpp"
#include "components.hpp"
#include "coordinator.hpp"
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

Coordinator gCoordinator;

Game::Game(Client &client)
    : _client(client), _scrollSpeed(500.0f), _connectionProtocol(client),
      positionUpdateTimer(0.0f), positionUpdateInterval(0.05f) {}

void Game::updateLoadingCircle(float deltaTime) {
    float rotationSpeed = 150.0f;
    float rotationDelta = rotationSpeed * deltaTime;
    sf::Vector2f center = loadingCircle[0].position;
    for (size_t i = 1; i < loadingCircle.getVertexCount(); ++i) {
        sf::Vector2f point = loadingCircle[i].position - center;
        float angle = atan2(point.y, point.x);
        float distance = sqrt(point.x * point.x + point.y * point.y);
        angle += rotationDelta * (M_PI / 180.0f);
        loadingCircle[i].position =
            center + sf::Vector2f(cos(angle) * distance, sin(angle) * distance);
    }
}

void Game::update(float deltaTime) {
    inputSystem->update();
    if (isWaiting) {
        checkPlayersInWaitingArea();
        updateLoadingCircle(deltaTime);
    }
    if (isFading) {
        if (fadeIn) {
            fadeAlpha += fadeSpeed * deltaTime;
            if (fadeAlpha >= 255.0f) {
                fadeAlpha = 255.0f;
                isFading = false;
            }
        } else {
            fadeAlpha -= fadeSpeed * deltaTime;
            if (fadeAlpha <= 0.0f) {
                fadeAlpha = 0.0f;
                isFading = false;
                if (showStageText && !isTextFading) {
                    isTextFading = true;
                    textFadeAlpha = 0.0f;
                }
            }
        }
        fadeOverlay.setFillColor(
            sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeAlpha)));
    }
    if (isTextFading) {
        textFadeAlpha += textFadeSpeed * deltaTime;
        if (textFadeAlpha >= 255.0f) {
            textFadeAlpha = 255.0f;
            isTextFading = false;
            showStageText = false;
            stageTextTimer = 0.0f;
        }
        stageText.setFillColor(
            sf::Color(255, 255, 255, static_cast<sf::Uint8>(textFadeAlpha)));
    }
    for (auto it = entityLastUpdateTime.begin();
         it != entityLastUpdateTime.end();) {
        it->second += deltaTime;

        if (it->second >= entityTimeout) {
            uint32_t entityId = it->first;
            auto entityIt = gameEntities.find(entityId);
            if (entityIt != gameEntities.end()) {
                handleEntityDestroy(entityId);
                it = entityLastUpdateTime.erase(it);
                continue;
            } else {
                it = entityLastUpdateTime.erase(it);
                continue;
            }
        }
        ++it;
    }
    for (auto &[clientId, interpData] : interpolationDataMap) {
        if (clientEntities.find(clientId) == clientEntities.end()) {
            continue;
        }
        Entity entity = clientEntities[clientId];
        auto &transform = gCoordinator.getComponent<TransformComponent>(entity);
        interpData.elapsedTime += deltaTime;
        if (interpData.elapsedTime > interpData.totalTime) {
            interpData.elapsedTime = interpData.totalTime;
        }
        float t = interpData.elapsedTime / interpData.totalTime;
        transform.position =
            interpData.startPosition +
            t * (interpData.targetPosition - interpData.startPosition);
    }

    auto &input = gCoordinator.getComponent<InputComponent>(playerEntity);
    auto &velocity = gCoordinator.getComponent<VelocityComponent>(playerEntity);
    auto &transform =
        gCoordinator.getComponent<TransformComponent>(playerEntity);
    float movementSpeed = 200.0f;

    velocity.velocity.x = 0.0f;
    velocity.velocity.y = 0.0f;
    Direction direction;
    if (input.up) {
        velocity.velocity.y -= movementSpeed;
        direction = Direction::UP;
    }
    if (input.down) {
        velocity.velocity.y += movementSpeed;
        direction = Direction::DOWN;
    }
    if (input.left) {
        velocity.velocity.x -= movementSpeed;
        direction = Direction::LEFT;
    }
    if (input.right) {
        velocity.velocity.x += movementSpeed;
        direction = Direction::RIGHT;
    }
    missileCooldownTimer += deltaTime;
    if (input.pressed && missileCooldownTimer >= missileCooldownInterval) {
        _client.getConnectionProtocol()->sendAction(
            static_cast<uint8_t>(ActionType::Fire));
        laserSystem->addLaserEffect(playerEntity);
        missileCooldownTimer = 0.0f;
        _laserSound.play();
    }

    if (velocity.velocity.x != 0.0f && velocity.velocity.y != 0.0f) {
        velocity.velocity.x /= std::sqrt(2.0f);
        velocity.velocity.y /= std::sqrt(2.0f);
    }

    if (_client.getConnectionProtocol()->hasNewStageMessage()) {
        int newStage = _client.getConnectionProtocol()->getStageMessage();
        std::cout << "Received stage change message: " << newStage << std::endl;
        handleStageChange(newStage);
    }
    explosionSystem->update(deltaTime);
    setAnimationFrame(playerEntity,
                      sf::Vector2i(velocity.velocity.x, velocity.velocity.y));
    transform.position += velocity.velocity * deltaTime;
    for (const auto &[entityId, entity] : gameEntities) {
        auto &typeComponent = gCoordinator.getComponent<TypeComponent>(entity);
        if (typeComponent.type == EntityType::Monster) {
            animateMonster(entity, deltaTime);
        }
        if (typeComponent.type == EntityType::PowerUp) {
            animatePowerUp(entity, deltaTime);
        }
    }
    float minX = 0.0f;
    float maxX = 1920.0f - (33 * transform.scale.x);
    float minY = 0.0f;
    float maxY = 1080.0f - (17 * transform.scale.y);

    if (transform.position.x < minX) {
        transform.position.x = minX;
    } else if (transform.position.x > maxX) {
        transform.position.x = maxX;
    }

    if (transform.position.y < minY) {
        transform.position.y = minY;
    } else if (transform.position.y > maxY) {
        transform.position.y = maxY;
    }
    movementSystem->update(deltaTime);
    laserSystem->update(deltaTime);

    positionUpdateTimer += deltaTime;
    if (positionUpdateTimer >= positionUpdateInterval) {
        auto &transform =
            gCoordinator.getComponent<TransformComponent>(playerEntity);
        _client.getConnectionProtocol()->sendPositionUpdate(
            transform.position.x, transform.position.y, velocity.velocity.x,
            velocity.velocity.y, direction);
        positionUpdateTimer = 0.0f;
    }

    _gameView.move(-_scrollSpeed * deltaTime, 0);
    float viewLeftEdge = _gameView.getCenter().x - (_gameView.getSize().x / 2);
    if (viewLeftEdge <= 0) {
        _gameView.setCenter(_gameView.getCenter().x + _backgroundSize.x,
                            _gameView.getCenter().y);
    }
    bool isEscapePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
    if (isEscapePressed && !_wasEscapePressed) {
        _isQuitButtonVisible = !_isQuitButtonVisible;
    }
    _wasEscapePressed = isEscapePressed;

    if (_isQuitButtonVisible && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePixelPos =
            sf::Mouse::getPosition(_client.getWindow());
        sf::Vector2f mousePos =
            _client.getWindow().mapPixelToCoords(mousePixelPos);
        if (_quitButtonSprite.getGlobalBounds().contains(mousePos)) {
            handleQuitButtonClick(mousePos);
        }
    }
}

void Game::removeClientEntity(int clientId) {
    if (clientEntities.find(clientId) != clientEntities.end()) {
        gCoordinator.destroyEntity(clientEntities[clientId]);
        clientEntities.erase(clientId);
        playerNames.erase(clientId);
        interpolationDataMap.erase(clientId);
    }
}

void Game::render(sf::RenderWindow &window) {
    sf::Vector2f viewCenter = _gameView.getCenter();
    sf::Vector2f viewSize = _gameView.getSize();
    float viewLeftEdge = viewCenter.x - (viewSize.x / 2);
    currentBackgroundSprite.setPosition(viewLeftEdge - _backgroundSize.x, 0);
    window.draw(currentBackgroundSprite);
    currentBackgroundSprite.setPosition(viewLeftEdge, 0);
    window.draw(currentBackgroundSprite);
    currentBackgroundSprite.setPosition(viewLeftEdge + _backgroundSize.x, 0);
    window.draw(currentBackgroundSprite);
    renderSystem->render(window);
    laserSystem->render(window);
    nameRenderSystem->render(window, font);
    healthSystem->render(window, playerEntity);
    levelSystem->render(window, playerEntity, font);
    scoreSystem->render(window, font, playerEntity);
    if (isWaiting) {
        window.draw(waitingOverlay);
        window.draw(waitingText);
        window.draw(waitingArea);
        window.draw(loadingCircle);
    }
    if (isFading || fadeAlpha > 0) {
        window.draw(fadeOverlay);
    }
    if (showStageText) {
        window.draw(stageText);
    }
    if (_isQuitButtonVisible) {
        sf::RectangleShape overlay(sf::Vector2f(1920, 1080));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
        window.draw(_quitButtonSprite);
    }
}
