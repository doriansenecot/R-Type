/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** server_game_logic.cpp
*/

#include "server.hpp"

void Server::handleClientAction(int clientId, const ActionMessage &actionMsg) {
    switch (static_cast<ActionType>(actionMsg.actionType)) {
    case ActionType::Fire: {
        float posX = 0;
        float posY = 0;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            auto it = clients.find(clientId);
            if (it != clients.end() && it->second->authenticated) {
                posX = it->second->posX;
                posY = it->second->posY;
            }
        }
        Entity playerEntity = -1;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            auto it = clients.find(clientId);
            if (it != clients.end()) {
                playerEntity = it->second->playerEntity;
            }
        }

        LevelComponent *level = nullptr;
        if (playerEntity != -1 &&
            entityManager.hasComponent<LevelComponent>(playerEntity))
            level = &entityManager.getComponent<LevelComponent>(playerEntity);
        Entity missileEntity = entityManager.create_entity();
        entityManager.addComponent<TypeComponent>(
            missileEntity, TypeComponent{EntityType::Missile});
        entityManager.addComponent<PositionComponent>(
            missileEntity, PositionComponent{posX + 45.0f, posY - 12.0f});
        entityManager.addComponent<VelocityComponent>(
            missileEntity,
            VelocityComponent{sf::Vector2<float>(MISSILE_SPEED_X + 15, 0)});
        entityManager.addComponent<OwnerComponent>(missileEntity,
                                                   OwnerComponent{clientId});
        entityManager.addComponent<MonsterType>(missileEntity, MonsterType{-1});
        entityManager.addComponent<HealthComponent>(missileEntity,
                                                    HealthComponent{-1, -1});
        int damage =
            static_cast<int>(std::floor(level->currentLevel / 2.0f)) + 1;
        entityManager.addComponent<DamageComponent>(missileEntity,
                                                    DamageComponent{damage});
        sendEntitySpawn(missileEntity);
        sendActionMessage(clientId);
        break;
    }
    default:
        break;
    }
}

void Server::moveEntities() {
    auto &positions = entityManager.getComponentMap<PositionComponent>();
    std::vector<Entity> entitiesToDestroy;

    for (const auto &[entity, position] : positions) {
        if (entityManager.hasComponent<VelocityComponent>(entity)) {
            VelocityComponent &velocity =
                entityManager.getComponent<VelocityComponent>(entity);
            PositionComponent &pos =
                entityManager.getComponent<PositionComponent>(entity);

            if (entityManager.hasComponent<TypeComponent>(entity)) {
                auto &type = entityManager.getComponent<TypeComponent>(entity);
                monsterMovement(entity, pos, velocity, entityManager);
            }

            pos.x += velocity.velocity.x;
            pos.y += velocity.velocity.y;

            if (pos.y < 0) {
                pos.y = 0;
                velocity.velocity.y = std::abs(velocity.velocity.y);
            } else if (pos.y > 1000) {
                pos.y = 1000;
                velocity.velocity.y = -std::abs(velocity.velocity.y);
            }
            if (pos.x < 0 || pos.x > SCREEN_WIDTH) {
                if (entityManager.hasComponent<TypeComponent>(entity)) {
                    TypeComponent &type =
                        entityManager.getComponent<TypeComponent>(entity);
                    if (type.type == EntityType::Monster ||
                        type.type == EntityType::Missile) {
                        MonsterType &monsterType =
                            entityManager.getComponent<MonsterType>(entity);
                        if (monsterType.type == 10 || monsterType.type == 20 ||
                            monsterType.type == 30 || monsterType.type == 40) {
                            continue;
                        }
                        entitiesToDestroy.push_back(entity);
                    }
                }
            }
        }
    }

    for (Entity entity : entitiesToDestroy) {
        sendEntityDestroy(entity);
        entityManager.destroy_entity(entity);
    }
}

void Server::startGameLoop() {
    gameTimer.expires_after(std::chrono::milliseconds(16));
    gameTimer.async_wait([this](const std::error_code &ec) {
        if (!ec && running) {
            updateGame();
            startGameLoop();
        }
    });
}

void Server::spawnPowerUp(float posX, float posY) {
    Entity powerUpEntity = entityManager.create_entity();
    entityManager.addComponent<TypeComponent>(
        powerUpEntity, TypeComponent{EntityType::PowerUp});
    entityManager.addComponent<PositionComponent>(
        powerUpEntity, PositionComponent{posX, posY});
    entityManager.addComponent<VelocityComponent>(
        powerUpEntity, VelocityComponent{sf::Vector2<float>(0.0f, 0.0f)});
    entityManager.addComponent<OwnerComponent>(powerUpEntity,
                                               OwnerComponent{0});
    entityManager.addComponent<MonsterType>(powerUpEntity, MonsterType{-1});
    entityManager.addComponent<HealthComponent>(powerUpEntity,
                                                HealthComponent{-1, -1});
    entityManager.addComponent<DamageComponent>(powerUpEntity,
                                                DamageComponent{0});
    sendEntitySpawn(powerUpEntity);
}

void Server::update_life(Entity player, int hp, int offset) {
    if (entityManager.hasComponent<HealthComponent>(player)) {
        HealthComponent &health =
            entityManager.getComponent<HealthComponent>(player);
        if (offset == 0)
            health.currentHealth -= hp;
        else
            health.currentHealth += hp;
        if (health.currentHealth > health.maxHealth)
            health.currentHealth = health.maxHealth;
        if (health.currentHealth < 0)
            health.currentHealth = 0;
        sendHealthMessage(player, health, entityManager);
        if (health.currentHealth <= 0) {
            int clientId = 0;
            if (entityManager.hasComponent<OwnerComponent>(player)) {
                OwnerComponent &owner =
                    entityManager.getComponent<OwnerComponent>(player);
                clientId = owner.ownerId;
            }
            std::cout << "Player " << clientId << " died." << std::endl;
            sendDisconnectionMessage(clientId);
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                auto it = clients.find(clientId);
                if (it != clients.end()) {
                    it->second->alive = 0;
                }
            }
            entityManager.destroy_entity(player);
        }
    }
}

void Server::updateGame() {
    bool allReady = false;

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        size_t clientCount = clients.size();

        if (clientCount > 0) {
            allReady = true;

            for (const auto &[id, clientInfo] : clients) {
                if (!clientInfo->authenticated ||
                    clientInfo->playerEntity == -1) {
                    allReady = false;
                    break;
                }
                float posX = clientInfo->posX;
                float posY = clientInfo->posY;

                if (posX < waitingAreaPosX ||
                    posX > (waitingAreaPosX + waitingAreaWidth) ||
                    posY < waitingAreaPosY ||
                    posY > (waitingAreaPosY + waitingAreaHeight)) {
                    allReady = false;
                    break;
                }
            }
        } else {
        }
    }

    if (currentGameState == GameStateWithClients::Waiting) {
        if (allReady) {
            transitionToActive();
        } else {
            moveEntities();
            handleCollisions();
            sendEntityUpdates();
            return;
        }
    }

    if (currentGameState == GameStateWithClients::Active) {
        moveEntities();
        spawnMonsters();
        handleCollisions();
        sendEntityUpdates();
    }
}
