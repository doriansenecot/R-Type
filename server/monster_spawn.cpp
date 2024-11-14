/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** monster_spawn.cpp
*/

#include "server.hpp"

void Server::spawnMonsters() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    if (!firstPlayerConnected)
        return;

    auto now = std::chrono::steady_clock::now();
    auto timeSinceFirstPlayerConnected =
        std::chrono::duration_cast<std::chrono::seconds>(now -
                                                         firstPlayerConnectTime)
            .count();
    if (timeSinceFirstPlayerConnected < MONSTER_START_DELAY)
        return;

    auto durationSinceLastSpawn =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastMonsterSpawnTime)
            .count();
    if (durationSinceLastSpawn >= MONSTER_SPAWN_INTERVAL) {
        if (currentStage == 1)
            Spawnstage1(now);
        else if (currentStage == 2)
            Spawnstage2(now);
        else if (currentStage == 3)
            Spawnstage3(now);
        else if (currentStage == 4)
            Spawnstage4(now);
        else if (currentStage >= 5) {
            SpawnstageOther(now, 1);
        }
    }
}

void Server::monsterMovement(Entity entity, PositionComponent &pos,
                             VelocityComponent &velocity,
                             EntityManager &entityManager) {
    TypeComponent &type = entityManager.getComponent<TypeComponent>(entity);
    if (type.type == EntityType::Monster &&
        entityManager.hasComponent<BehaviorComponent>(entity)) {
        BehaviorComponent &behavior =
            entityManager.getComponent<BehaviorComponent>(entity);
        switch (behavior.movementPattern) {
        case MovementPattern::Zigzag:
            if (rand() % 30 == 0) {
                velocity.velocity.y = static_cast<float>(
                    (rand() % 2 == 0 ? -1 : 1) * (rand() % 5 + 2));
            }
            if (rand() % 60 == 0) {
                velocity.velocity.x =
                    static_cast<float>((rand() % 8 + 3) * -1.0f);
            }
            break;
        case MovementPattern::Wave:
            velocity.velocity.y = std::sin(pos.x / 50.0f) * 5.0f;
            break;
        case MovementPattern::Spiral:
            velocity.velocity.x = -std::abs(std::cos(pos.y / 50.0f) * 5.0f);
            velocity.velocity.y = std::sin(pos.x / 50.0f) * 5.0f;
            break;
        case MovementPattern::StraightLine:
            velocity.velocity.x = -5.0f;
            velocity.velocity.y = 0.0f;
            break;
        case MovementPattern::StraightLineSpeed:
            velocity.velocity.x = -15.0f;
            velocity.velocity.y = 0.0f;
            break;
        case MovementPattern::RandomBounce:
            if (rand() % 40 == 0) {
                velocity.velocity.x =
                    -std::abs(static_cast<float>((rand() % 15 + 7)));
                velocity.velocity.y = static_cast<float>(
                    (rand() % 2 == 0 ? -1 : 1) * (rand() % 8 + 3));
            }
            break;
        case MovementPattern::Hover:
            velocity.velocity.y = std::sin(pos.x / 100.0f) * 2.0f;
            velocity.velocity.x = -3.0f;
            break;
        case MovementPattern::Charge:
            if (rand() % 100 == 0) {
                velocity.velocity.x = -std::abs(static_cast<float>(10.0f));
                velocity.velocity.y =
                    static_cast<float>((rand() % 2 == 0 ? -1 : 1) * 10.0f);
            }
            break;
        case MovementPattern::Circular:
            velocity.velocity.x = -std::abs(std::cos(pos.y / 40.0f) * 4.0f);
            velocity.velocity.y = std::sin(pos.x / 40.0f) * 4.0f;
            break;
        case MovementPattern::RandomWalk:
            if (rand() % 5 == 0) {
                velocity.velocity.x =
                    -std::abs(static_cast<float>((rand() % 5 + 1)));
                velocity.velocity.y =
                    static_cast<float>((rand() % 3 - 1) * (rand() % 15 + 1));
            }
            break;
        case MovementPattern::Boss3: {
            float bossWidth = 224.2f;
            float bossHeight = 224.0f;
            float centerX = 1920.0f / 2.0f - bossWidth / 2.0f;
            float centerY = 1080.0f / 2.0f - bossHeight / 2.0f;
            float time = static_cast<float>(
                std::chrono::steady_clock::now().time_since_epoch().count() /
                1000000000.0f);
            float baseSpeed = 3.0f;
            float xMovement = std::sin(time * 0.5f) * baseSpeed * 7.0f;
            float yMovement = std::cos(time * 0.3f) * baseSpeed * 6.0f;
            float pullStrength = 0.02f;
            xMovement += (centerX - pos.x) * pullStrength;
            yMovement += (centerY - pos.y) * pullStrength;
            pos.x += xMovement;
            pos.y += yMovement;
            float bossLeftBoundary = 0.0f;
            float bossRightBoundary = 1920.0f - bossWidth;
            float bossTopBoundary = 0.0f;
            float bossBottomBoundary = 1080.0f - bossHeight;
            if (pos.x < bossLeftBoundary)
                pos.x = bossLeftBoundary;
            if (pos.x > bossRightBoundary)
                pos.x = bossRightBoundary;
            if (pos.y < bossTopBoundary)
                pos.y = bossTopBoundary;
            if (pos.y > bossBottomBoundary)
                pos.y = bossBottomBoundary;
            break;
        }
        case MovementPattern::Boss: {
            float bossWidth = 162.2f;
            float bossHeight = 330.0f;
            float bossLeftBoundary = (1920 - 1780) / 2.0f;
            float bossRightBoundary = bossLeftBoundary + 1780 - bossWidth;
            float bossTopBoundary = (1080 - 900) / 2.0f;
            float bossBottomBoundary = bossTopBoundary + 900 - bossHeight;
            float time = static_cast<float>(
                std::chrono::steady_clock::now().time_since_epoch().count() /
                1000000000.0f);
            float speed = 5.0f;
            float xMovement = std::sin(time) * speed * 4.0f;
            float yMovement = std::cos(time * 0.5f) * speed * 2.0f;
            pos.x += xMovement;
            pos.y += yMovement;
            if (pos.x < bossLeftBoundary)
                pos.x = bossLeftBoundary;
            if (pos.x > bossRightBoundary)
                pos.x = bossRightBoundary;
            if (pos.y < bossTopBoundary)
                pos.y = bossTopBoundary;
            if (pos.y > bossBottomBoundary)
                pos.y = bossBottomBoundary;
            break;
        }
        case MovementPattern::Boss2: {
            float bossWidth = 234.0f;
            float bossHeight = 351.0f;
            float bossLeftBoundary = (1920 - 1730) / 2.0f;
            float bossRightBoundary = bossLeftBoundary + 1620 - bossWidth;
            float bossTopBoundary = (1080 - 1200) / 2.0f;
            float bossBottomBoundary = bossTopBoundary + 1000 - bossHeight;
            float time = static_cast<float>(
                std::chrono::steady_clock::now().time_since_epoch().count() /
                1000000000.0f);
            float speed = 5.0f;
            float xMovement = std::sin(time) * speed * 4.0f;
            float yMovement = std::cos(time * 0.5f) * speed * 2.0f;
            pos.x += xMovement;
            pos.y += yMovement;
            if (pos.x < bossLeftBoundary)
                pos.x = bossLeftBoundary;
            if (pos.x > bossRightBoundary)
                pos.x = bossRightBoundary;
            if (pos.y < bossTopBoundary)
                pos.y = bossTopBoundary;
            if (pos.y > bossBottomBoundary)
                pos.y = bossBottomBoundary;
            break;
        }
        case MovementPattern::BossGround: {
            float bossRightBoundary = 864.0f;
            float leftBoundary = 50.0f;
            if (pos.x <= leftBoundary) {
                velocity.velocity.x = std::abs(velocity.velocity.x);
            }
            if (pos.x >= bossRightBoundary) {
                velocity.velocity.x = -std::abs(velocity.velocity.x);
            }
            if (rand() % 50 == 0) {
                velocity.velocity.x += (rand() % 3 - 1) * 5.0f;
                velocity.velocity.x =
                    std::clamp(velocity.velocity.x, -10.0f, 10.0f);
            }
            break;
        }
        default:
            break;
        }
    }
}

bool Server::noActiveMonsters() {
    auto &types = entityManager.getComponentMap<TypeComponent>();
    for (const auto &[entity, type] : types) {
        if (type.type == EntityType::Monster) {
            return false;
        }
    }
    return true;
}

void Server::handleMonsterKilled(Entity monsterEntity, int clientId) {
    MonsterType &monsterType =
        entityManager.getComponent<MonsterType>(monsterEntity);
    if (monsterType.type == 0) {
        clientScores[clientId] += 20 + difficulty * 2;
    } else if (monsterType.type == 1) {
        clientScores[clientId] += 50 + difficulty * 4;
    } else if (monsterType.type == 2) {
        clientScores[clientId] += 10 + difficulty * 2;
    } else if (monsterType.type == 3) {
        clientScores[clientId] += 30 + difficulty * 2;
    } else if (monsterType.type == 4) {
        clientScores[clientId] += 40 + difficulty * 3;
    } else if (monsterType.type == 5) {
        clientScores[clientId] += 60 + difficulty * 4;
    } else if (monsterType.type == 6) {
        clientScores[clientId] += 69 + difficulty * 4;
    } else if (monsterType.type == 7) {
        clientScores[clientId] += 45 + difficulty * 2;
    } else if (monsterType.type == 8) {
        clientScores[clientId] += 75 + difficulty * 4;
    } else if (monsterType.type == 10) {
        clientScores[clientId] += 200 + difficulty * 6;
    } else if (monsterType.type == 20) {
        clientScores[clientId] += 400 + difficulty * 8;
    } else if (monsterType.type == 30) {
        clientScores[clientId] += 500 + difficulty * 10;
    } else if (monsterType.type == 31) {
        clientScores[clientId] += 0;
    } else if (monsterType.type == 40) {
        clientScores[clientId] += 1000 + difficulty * 15;
    }
    sendAllScores();
}
