/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** collision.cpp
*/

#include "server.hpp"

int Server::modifyXp(MonsterType monster) {
    int xp = 0;
    if (monster.type == 0)
        xp += 2;
    else if (monster.type == 1)
        xp += 4;
    else if (monster.type == 2)
        xp += 6;
    else if (monster.type == 3)
        xp += 8;
    else if (monster.type == 4)
        xp += 10;
    else if (monster.type == 5)
        xp += 12;
    else if (monster.type == 6)
        xp += 14;
    else if (monster.type == 7)
        xp += 16;
    else if (monster.type == 8)
        xp += 18;
    else if (monster.type == 10)
        xp += 20;
    else if (monster.type == 20)
        xp += 30;
    else if (monster.type == 30)
        xp += 40;
    else if (monster.type == 31)
        xp += 0;
    else if (monster.type == 40)
        xp += 50;
    return xp;
}

void Server::missileMonster(
    std::vector<std::pair<Entity, Entity>> &missileMonsterCollisions,
    std::unordered_map<Entity, PositionComponent> &positions) {
    for (auto &[missile, monster] : missileMonsterCollisions) {
        float posX = positions[monster].x;
        float posY = positions[monster].y;
        int damage =
            entityManager.getComponent<DamageComponent>(missile).damage;
        entityManager.getComponent<HealthComponent>(monster).currentHealth -=
            damage;
        int ownerId = 0;
        if (entityManager.hasComponent<OwnerComponent>(missile)) {
            ownerId =
                entityManager.getComponent<OwnerComponent>(missile).ownerId;
        }
        Entity playerEntity = -1;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            auto it = clients.find(ownerId);
            if (it != clients.end()) {
                playerEntity = it->second->playerEntity;
            }
        }
        if (entityManager.getComponent<HealthComponent>(monster)
                    .currentHealth <= 0 &&
            playerEntity != -1) {
            if (entityManager.hasComponent<XPComponent>(playerEntity)) {
                XPComponent &xp =
                    entityManager.getComponent<XPComponent>(playerEntity);
                xp.currentXP +=
                    modifyXp(entityManager.getComponent<MonsterType>(monster));
                bool leveledUp = false;
                if (xp.currentXP >= 100) {
                    xp.currentXP -= 100;
                    leveledUp = true;
                }
                int currentLevel = 1;
                if (entityManager.hasComponent<LevelComponent>(playerEntity)) {
                    LevelComponent &level =
                        entityManager.getComponent<LevelComponent>(
                            playerEntity);
                    HealthComponent &health =
                        entityManager.getComponent<HealthComponent>(
                            playerEntity);
                    if (leveledUp) {
                        level.currentLevel += 1;
                        currentLevel = level.currentLevel;
                        health.maxHealth += 10;
                        update_life(playerEntity, 20, 1);
                    } else {
                        currentLevel = level.currentLevel;
                    }
                }
                sendXPMessages(ownerId, xp.currentXP, currentLevel);
            }
            MonsterType &monsterType =
                entityManager.getComponent<MonsterType>(monster);
            if (monsterType.type == 10 || monsterType.type == 20 ||
                monsterType.type == 30 || monsterType.type == 40) {
                bossSpawned = false;
                monstersSpawned++;
            }
            handleMonsterKilled(monster, ownerId);
            entityManager.destroy_entity(monster);
            sendEntityDestroy(monster);
            if (rand() % 2 == 0) {
                spawnPowerUp(posX, posY);
            }
        }
        entityManager.destroy_entity(missile);
        sendEntityDestroy(missile);
    }
}

void Server::playerPowerUp(
    std::vector<std::pair<Entity, Entity>> &playerPowerUpCollisions) {
    for (auto &[player, powerUp] : playerPowerUpCollisions) {
        int bonus = rand() % 2;
        if (bonus == 0) {
            int healthBonus = 5 + rand() % 9;
            update_life(player, healthBonus, 1);
        } else if (bonus == 1) {
            if (entityManager.hasComponent<XPComponent>(player)) {
                XPComponent &xp =
                    entityManager.getComponent<XPComponent>(player);
                int xpBonus = 5 + rand() % 26;
                xp.currentXP += xpBonus;
                bool leveledUp = false;
                if (xp.currentXP >= 100) {
                    xp.currentXP -= 100;
                    leveledUp = true;
                }
                if (leveledUp &&
                    entityManager.hasComponent<LevelComponent>(player)) {
                    LevelComponent &level =
                        entityManager.getComponent<LevelComponent>(player);
                    HealthComponent &health =
                        entityManager.getComponent<HealthComponent>(player);
                    level.currentLevel += 1;
                    health.maxHealth += 10;
                    update_life(player, 20, 1);
                }
                sendXPMessages(
                    entityManager.getComponent<OwnerComponent>(player).ownerId,
                    xp.currentXP,
                    entityManager.getComponent<LevelComponent>(player)
                        .currentLevel);
            }
        }
        entityManager.destroy_entity(powerUp);
        sendEntityDestroy(powerUp);
    }
}

void Server::lifeModify(Entity player, MonsterType monsterdeg) {
    if (monsterdeg.type == 0)
        update_life(player, difficulty + 2, 0);
    else if (monsterdeg.type == 1)
        update_life(player, difficulty + 10, 0);
    else if (monsterdeg.type == 2)
        update_life(player, difficulty + 3, 0);
    else if (monsterdeg.type == 3)
        update_life(player, difficulty + 2, 0);
    else if (monsterdeg.type == 4)
        update_life(player, difficulty + 4, 0);
    else if (monsterdeg.type == 5)
        update_life(player, difficulty + 5, 0);
    else if (monsterdeg.type == 6)
        update_life(player, difficulty + 3, 0);
    else if (monsterdeg.type == 7)
        update_life(player, difficulty + 20, 0);
    else if (monsterdeg.type == 8)
        update_life(player, difficulty + 5, 0);
    else if (monsterdeg.type == 10)
        update_life(player, 3, 0);
    else if (monsterdeg.type == 20)
        update_life(player, 1, 0);
    else if (monsterdeg.type == 30)
        update_life(player, 2, 0);
    else if (monsterdeg.type == 31)
        update_life(player, difficulty + 6, 0);
    else if (monsterdeg.type == 40)
        update_life(player, 5, 0);
}

void Server::handleCollisions() {
    auto &positions = entityManager.getComponentMap<PositionComponent>();
    auto &types = entityManager.getComponentMap<TypeComponent>();
    auto &velocities = entityManager.getComponentMap<VelocityComponent>();
    auto &owners = entityManager.getComponentMap<OwnerComponent>();

    std::vector<std::pair<Entity, Entity>> missileMonsterCollisions;
    std::vector<std::pair<Entity, Entity>> monsterPlayerCollisions;
    std::vector<std::pair<Entity, Entity>> playerPowerUpCollisions;

    std::vector<Entity> missiles;
    std::vector<Entity> monsters;
    std::vector<Entity> players;
    std::vector<Entity> powerUps;

    for (const auto &[entity, type] : types) {
        if (type.type == EntityType::Missile)
            missiles.push_back(entity);
        else if (type.type == EntityType::Monster)
            monsters.push_back(entity);
        else if (type.type == EntityType::Player)
            players.push_back(entity);
        else if (type.type == EntityType::PowerUp)
            powerUps.push_back(entity);
    }

    for (auto missile : missiles) {
        auto &missilePos = positions[missile];
        sf::FloatRect missileBounds(missilePos.x, missilePos.y, 18.0f, 18.0f);
        for (auto monster : monsters) {
            auto &monsterPos = positions[monster];
            SizeComponent &monsterSize =
                entityManager.getComponent<SizeComponent>(monster);
            sf::FloatRect monsterBounds(monsterPos.x, monsterPos.y,
                                        monsterSize.width, monsterSize.height);
            if (missileBounds.intersects(monsterBounds))
                missileMonsterCollisions.emplace_back(missile, monster);
        }
    }

    for (auto monster : monsters) {
        auto &monsterPos = positions[monster];
        SizeComponent &monsterSize =
            entityManager.getComponent<SizeComponent>(monster);
        sf::FloatRect monsterBounds(monsterPos.x, monsterPos.y,
                                    monsterSize.width, monsterSize.height);
        for (auto player : players) {
            auto &playerPos = positions[player];
            sf::FloatRect playerBounds(playerPos.x, playerPos.y, 83.0f, 42.5f);
            if (monsterBounds.intersects(playerBounds)) {
                monsterPlayerCollisions.emplace_back(monster, player);
            }
        }
    }

    for (auto player : players) {
        auto &playerPos = positions[player];
        sf::FloatRect playerBounds(playerPos.x, playerPos.y, 83.0f, 42.5f);
        for (auto powerUp : powerUps) {
            auto &powerUpPos = positions[powerUp];
            sf::FloatRect powerUpBounds(powerUpPos.x, powerUpPos.y, 32.0f,
                                        32.0f);
            if (playerBounds.intersects(powerUpBounds)) {
                playerPowerUpCollisions.emplace_back(player, powerUp);
            }
        }
    }
    missileMonster(missileMonsterCollisions, positions);
    playerPowerUp(playerPowerUpCollisions);

    for (auto &[monster, player] : monsterPlayerCollisions) {
        if (entityManager.hasComponent<MonsterType>(monster)) {
            MonsterType &monsterdeg =
                entityManager.getComponent<MonsterType>(monster);
            lifeModify(player, monsterdeg);
            if (monsterdeg.type != 10 && monsterdeg.type != 20 &&
                monsterdeg.type != 30 && monsterdeg.type != 40) {
                entityManager.destroy_entity(monster);
                sendEntityDestroy(monster);
            }
            monstersKilled++;
        }
    }
    if (monstersSpawned >= monstersToSpawn) {
        int nextStage = currentStage + 1;
        handleStageChange(nextStage);
        monstersSpawned = 0;
    }
}
