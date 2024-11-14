/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** game_handle
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

void Game::handlePlayerInfo(int clientId, const std::string &name) {
    playerNames[clientId] = name;

    Entity entity;
    if (clientId == _client.clientId) {
        entity = playerEntity;
    } else if (clientEntities.find(clientId) != clientEntities.end()) {
        entity = clientEntities[clientId];
    } else {
        return;
    }

    if (gCoordinator.hasComponent<NameComponent>(entity)) {
        auto &nameComp = gCoordinator.getComponent<NameComponent>(entity);
        nameComp.name = name;
    } else {
        NameComponent nameComp;
        nameComp.name = name;
        gCoordinator.addComponent(entity, nameComp);
    }
}

void Game::handleScoreUpdate(uint8_t clientId, int newScore) {
    if (scoreSystem) {
        scoreSystem->updateScore(clientId, newScore);
    }
}

void Game::handleEntitySpawn(uint32_t entityId, EntityType entityType,
                             float posX, float posY, float velocityX,
                             float velocityY, uint8_t ownerId,
                             int monsterType) {
    if (gameEntities.find(entityId) != gameEntities.end()) {
        handleEntityUpdate(entityId, posX, posY);
        return;
    }

    Entity newEntity = gCoordinator.createEntity();
    gameEntities[entityId] = newEntity;

    TypeComponent typeComp;
    typeComp.type = entityType;
    gCoordinator.addComponent(newEntity, typeComp);

    switch (entityType) {
    case EntityType::Monster:
        initializeMonster(newEntity, posX, posY, velocityX, velocityY,
                          monsterType);
        break;
    case EntityType::Missile:
        initializeMissile(newEntity, posX, posY);
        break;
    case EntityType::PowerUp:
        initializePowerUp(newEntity, posX, posY);
        break;
    default:
        std::cerr << "Unknown entity type: " << static_cast<int>(entityType)
                  << std::endl;
        break;
    }

    if (entityType != EntityType::Player && entityType != EntityType::PowerUp) {
        entityLastUpdateTime[entityId] = 0.0f;
    }
}

void Game::handleActionMessage(const ActionMessage &msg) {
    if (msg.actionType == static_cast<uint8_t>(ActionType::Fire)) {
        int clientId = msg.header.clientId;
        Entity entity = getEntityByClientId(clientId);
        if (entity != -1) {
            laserSystem->addLaserEffect(entity);
            _laserSound.play();
            _laserSound.setVolume(30.0f);
        }
    }
}

void Game::handleEntityUpdate(uint32_t entityId, float posX, float posY) {
    auto it = gameEntities.find(entityId);
    if (it != gameEntities.end()) {
        Entity entity = it->second;
        auto &transform = gCoordinator.getComponent<TransformComponent>(entity);
        transform.position = sf::Vector2f(posX, posY);
        entityLastUpdateTime[entityId] = 0.0f;
    }
}

void Game::handleExperienceUpdate(int entityId, int currentXP,
                                  int currentLevel) {
    Entity entity = getEntityByClientId(entityId);
    if (_client.clientId == entityId) {
        if (gCoordinator.hasComponent<XPComponent>(playerEntity)) {
            XPComponent &xp =
                gCoordinator.getComponent<XPComponent>(playerEntity);
            xp.currentXP = currentXP;
        } else {
            XPComponent xp;
            xp.currentXP = currentXP;
            gCoordinator.addComponent(playerEntity, xp);
        }
        if (gCoordinator.hasComponent<LevelComponent>(playerEntity)) {
            LevelComponent &level =
                gCoordinator.getComponent<LevelComponent>(playerEntity);
            level.currentLevel = currentLevel;
        } else {
            LevelComponent level;
            level.currentLevel = currentLevel;
            gCoordinator.addComponent(playerEntity, level);
        }
        std::cout << "Updated Player " << entityId << " to Level "
                  << currentLevel << " with XP " << currentXP << std::endl;
    }
    if (entity == -1) {
        return;
    }
    if (gCoordinator.hasComponent<LevelComponent>(entity)) {
        LevelComponent &level =
            gCoordinator.getComponent<LevelComponent>(entity);
        level.currentLevel = currentLevel;
    } else {
        LevelComponent level;
        level.currentLevel = currentLevel;
        gCoordinator.addComponent(entity, level);
    }
    if (gCoordinator.hasComponent<XPComponent>(entity)) {
        XPComponent &xp = gCoordinator.getComponent<XPComponent>(entity);
        xp.currentXP = currentXP;
    } else {
        XPComponent xp;
        xp.currentXP = currentXP;
        gCoordinator.addComponent(entity, xp);
    }
}

void Game::handlePositionUpdate(int clientId, float posX, float posY,
                                float velocityX, float velocityY) {
    if (clientId == _client.clientId) {
        return;
    }
    if (clientEntities.find(clientId) == clientEntities.end()) {
        Entity clientEntity = gCoordinator.createEntity();
        sf::IntRect spaceshipTextureRect = getTextureRectForClientID(clientId);
        setupSpaceship(clientEntity, sf::Vector2f(posX, posY),
                       spaceshipTextureRect, playerNames[clientId]);
        clientEntities[clientId] = clientEntity;
    }

    Entity entity = clientEntities[clientId];
    auto &transform = gCoordinator.getComponent<TransformComponent>(entity);
    InterpolationData &interpData = interpolationDataMap[clientId];
    interpData.startPosition = transform.position;
    interpData.targetPosition = sf::Vector2f(posX, posY);
    interpData.elapsedTime = 0.0f;
    interpData.totalTime = positionUpdateInterval;
    auto &input = gCoordinator.getComponent<InputComponent>(entity);
    auto &sprite = gCoordinator.getComponent<SpriteComponent>(entity);
    setAnimationFrame(entity, sf::Vector2i(static_cast<int>(velocityX),
                                           static_cast<int>(velocityY)));
}

void Game::handleEntityDestroy(uint32_t entityId) {
    auto it = gameEntities.find(entityId);
    if (it != gameEntities.end()) {
        Entity entity = it->second;
        if (gCoordinator.hasComponent<TypeComponent>(entity)) {
            TypeComponent typeComp =
                gCoordinator.getComponent<TypeComponent>(entity);
            if (typeComp.type == EntityType::Monster) {
                _explosionSound.play();
                _explosionSound.setVolume(30.0f);

                Entity explosionEntity = gCoordinator.createEntity();
                auto &transform =
                    gCoordinator.getComponent<TransformComponent>(entity);

                TransformComponent explosionTransform;
                explosionTransform.position = transform.position;
                explosionTransform.scale = sf::Vector2f(1.5f, 1.5f);
                gCoordinator.addComponent(explosionEntity, explosionTransform);

                SpriteComponent explosionSprite;
                explosionSprite.sprite.setTexture(explosionTexture);
                explosionSprite.sprite.setTextureRect(
                    sf::IntRect(0, 98, 66, 65));
                gCoordinator.addComponent(explosionEntity, explosionSprite);

                AnimationComponent explosionAnimation;
                explosionAnimation.frameTime = 0.1f;
                explosionAnimation.numFrames = 5;
                explosionAnimation.frameIndex = 0;
                explosionAnimation.monsterType = 1000;
                gCoordinator.addComponent(explosionEntity, explosionAnimation);
            }
        }
        gCoordinator.destroyEntity(entity);
        gameEntities.erase(it);

        for (auto clientIt = clientEntities.begin();
             clientIt != clientEntities.end();) {
            if (clientIt->second == entity) {
                clientIt = clientEntities.erase(clientIt);
            } else {
                ++clientIt;
            }
        }
    }
}

void Game::handleStageChange(int newStage) {
    PathHelper pathHelper;
    startFadeEffect(true);
    stageSystem.setStage(newStage <= 5 ? newStage : ((newStage - 2) % 3 + 2));
    const Stage &stageData = stageSystem.getCurrentStageData();
    if (!currentBackgroundTexture.loadFromFile(
            pathHelper.getImagePath(stageData.backgroundPath))) {
        std::cerr << "Failed to load stage background for stage " << newStage
                  << std::endl;
        return;
    }
    currentBackgroundSprite.setTexture(currentBackgroundTexture);

    for (auto &[clientId, entity] : clientEntities) {
        auto &transform = gCoordinator.getComponent<TransformComponent>(entity);
        transform.position = sf::Vector2f(400, 540);
    }
    auto &playerTransform =
        gCoordinator.getComponent<TransformComponent>(playerEntity);
    playerTransform.position = sf::Vector2f(400, 540);
    stageText.setString("Stage " + std::to_string(newStage));
    stageText.setOrigin(stageText.getLocalBounds().width / 2,
                        stageText.getLocalBounds().height / 2);
    showStageText = true;
    stageTextTimer = 0.0f;
    startFadeEffect(false);
}

void Game::setPlayerIsDead(bool isDead) {
    playerIsDead = true;
    auto &sprite =
        gCoordinator.getComponent<SpriteComponent>(playerEntity);
    sprite.sprite.setColor(sf::Color(255, 255, 255, 128));
}

void Game::handleHealthUpdate(int clientId, int currentHealth, int maxHealth) {
    std::cout << "Received health update for entity " << clientId
              << " with current health " << currentHealth << " and max health "
              << maxHealth << std::endl;
    Entity entity = getEntityByClientId(clientId);
    if (_client.clientId == clientId) {
        if (gCoordinator.hasComponent<HealthComponent>(playerEntity)) {
            HealthComponent &health =
                gCoordinator.getComponent<HealthComponent>(playerEntity);
            health.currentHealth = currentHealth;
            health.maxHealth = maxHealth;
            if (health.currentHealth <= 0) {
                setPlayerIsDead(true);
            }
        }
        return;
    }
    if (entity == -1) {
        std::cerr
            << "Error: Invalid entity received for health update (clientId: "
            << clientId << ")" << std::endl;
        return;
    }
    if (gCoordinator.hasComponent<HealthComponent>(entity)) {
        HealthComponent &health =
            gCoordinator.getComponent<HealthComponent>(entity);
        health.currentHealth = currentHealth;
        health.maxHealth = maxHealth;
    }
}

void Game::handleQuitButtonClick(const sf::Vector2f &mousePos) {
    _client.getWindow().close();
    _isQuitButtonVisible = false;
}
