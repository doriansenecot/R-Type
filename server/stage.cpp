/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** stage
*/

#include "server.hpp"

void Server::Spawnstage1(std::chrono::steady_clock::time_point now) {
    float posX = SCREEN_WIDTH;
    float posY = static_cast<float>(rand() % 1000);
    int monsterTypes = rand() % 3;
    float velocityX;
    float velocityY = 0.0f;

    if (monsterTypes == 0) {
        velocityX = MONSTER_SPEED_X * 3.0;
    } else if (monsterTypes == 1) {
        velocityX = MONSTER_SPEED_X * 4.5f;
    } else if (monsterTypes == 2) {
        velocityX = MONSTER_SPEED_X * 2.5f;
    }

    if (monstersSpawned < monstersToSpawn - 1) {
        Entity monsterEntity = entityManager.create_entity();
        MonsterType monsterType;
        monsterType.type = monsterTypes;
        entityManager.addComponent<TypeComponent>(
            monsterEntity, TypeComponent{EntityType::Monster});
        entityManager.addComponent<PositionComponent>(
            monsterEntity, PositionComponent{posX, posY});
        entityManager.addComponent<VelocityComponent>(
            monsterEntity,
            VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
        entityManager.addComponent<OwnerComponent>(monsterEntity,
                                                   OwnerComponent{0});
        entityManager.addComponent<MonsterType>(monsterEntity,
                                                MonsterType{monsterType});

        BehaviorComponent behavior;
        SizeComponent size;
        if (monsterType.type == 0) {
            behavior.movementPattern = MovementPattern::Wave;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{2, 2});

            size.width = 82.5f;
            size.height = 90.0f;
        } else if (monsterType.type == 1) {
            behavior.movementPattern = MovementPattern::StraightLine;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 162.5f;
            size.height = 125.0f;
        } else if (monsterType.type == 2) {
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{1, 1});

            size.width = 85.0f;
            size.height = 82.5f;
        }
        entityManager.addComponent<BehaviorComponent>(monsterEntity, behavior);
        entityManager.addComponent<SizeComponent>(monsterEntity, size);

        entityManager.addComponent<DamageComponent>(monsterEntity,
                                                    DamageComponent{0});

        sendEntitySpawn(monsterEntity);

        lastMonsterSpawnTime = now;
        monstersSpawned++;
    } else if (!bossSpawned && noActiveMonsters()) {
        spawnBoss1(posX, posY, velocityX, velocityY);
    }
}

void Server::Spawnstage2(std::chrono::steady_clock::time_point now) {
    float posX = SCREEN_WIDTH;
    float posY = static_cast<float>(rand() % 1000);
    int monsterTypes = rand() % 4 + 1;
    float velocityX;
    float velocityY = 0.0f;

    if (monsterTypes == 2) {
        velocityX = MONSTER_SPEED_X * 2.5f;
    } else if (monsterTypes == 3) {
        velocityX = MONSTER_SPEED_X * 4.5f;
    } else if (monsterTypes == 4) {
        velocityX = MONSTER_SPEED_X * 5.0;
    } else if (monsterTypes == 1) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    }

    if (monstersSpawned < monstersToSpawn - 1) {
        Entity monsterEntity = entityManager.create_entity();
        MonsterType monsterType;
        monsterType.type = monsterTypes;
        entityManager.addComponent<TypeComponent>(
            monsterEntity, TypeComponent{EntityType::Monster});
        entityManager.addComponent<VelocityComponent>(
            monsterEntity,
            VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
        entityManager.addComponent<OwnerComponent>(monsterEntity,
                                                   OwnerComponent{0});
        entityManager.addComponent<MonsterType>(monsterEntity,
                                                MonsterType{monsterType});

        BehaviorComponent behavior;
        SizeComponent size;
        if (monsterType.type == 1) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::StraightLine;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 162.5f;
            size.height = 125.0f;
        } else if (monsterType.type == 2) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{1, 1});

            size.width = 85.0f;
            size.height = 82.5f;
        } else if (monsterType.type == 3) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, 931.5f});
            behavior.movementPattern = MovementPattern::StraightLine;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{2, 2});

            size.width = 148.5f;
            size.height = 148.5f;
        } else if (monsterType.type == 4) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 115.5f;
            size.height = 115.5f;
        }
        entityManager.addComponent<BehaviorComponent>(monsterEntity, behavior);
        entityManager.addComponent<SizeComponent>(monsterEntity, size);

        entityManager.addComponent<DamageComponent>(monsterEntity,
                                                    DamageComponent{0});

        sendEntitySpawn(monsterEntity);

        lastMonsterSpawnTime = now;
        monstersSpawned++;
    } else if (!bossSpawned && noActiveMonsters()) {
        spawnBoss2(velocityX, velocityY);
    } else if (bossSpawned) {
        spawnBoss2Mignon(posX, posY, velocityX, velocityY, now);
    }
}

void Server::Spawnstage3(std::chrono::steady_clock::time_point now) {
    float posX = SCREEN_WIDTH;
    float posY = static_cast<float>(rand() % 1000);
    int monsterTypesArray[] = {0, 3, 5, 6, 7, 8};
    int monsterTypes = monsterTypesArray[rand() % 6];
    float velocityX;
    float velocityY = 0.0f;

    if (monsterTypes == 0) {
        velocityX = MONSTER_SPEED_X * 4.5f;
    } else if (monsterTypes == 3) {
        velocityX = MONSTER_SPEED_X * 4.5f;
    } else if (monsterTypes == 5) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    } else if (monsterTypes == 6) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    } else if (monsterTypes == 7) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    } else if (monsterTypes == 8) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    }

    if (monstersSpawned < monstersToSpawn - 1) {
        Entity monsterEntity = entityManager.create_entity();
        MonsterType monsterType;
        monsterType.type = monsterTypes;
        entityManager.addComponent<TypeComponent>(
            monsterEntity, TypeComponent{EntityType::Monster});
        entityManager.addComponent<VelocityComponent>(
            monsterEntity,
            VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
        entityManager.addComponent<OwnerComponent>(monsterEntity,
                                                   OwnerComponent{0});
        entityManager.addComponent<MonsterType>(monsterEntity,
                                                MonsterType{monsterType});

        BehaviorComponent behavior;
        SizeComponent size;
        if (monsterType.type == 0) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Zigzag;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 82.5f;
            size.height = 90.0f;
        } else if (monsterType.type == 3) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, 931.5f});
            behavior.movementPattern = MovementPattern::StraightLine;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{2, 2});

            size.width = 148.5f;
            size.height = 148.5f;
        } else if (monsterType.type == 5) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::RandomWalk;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 99.0f;
            size.height = 88.0f;
        } else if (monsterType.type == 6) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Hover;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 82.5f;
            size.height = 82.5f;
        } else if (monsterType.type == 7) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, 1026.0f});
            behavior.movementPattern = MovementPattern::StraightLineSpeed;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 75.0f;
            size.height = 75.0f;
        } else if (monsterType.type == 8) {
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(monsterEntity,
                                                        HealthComponent{4, 4});

            size.width = 181.5f;
            size.height = 181.5f;
        }
        entityManager.addComponent<BehaviorComponent>(monsterEntity, behavior);
        entityManager.addComponent<SizeComponent>(monsterEntity, size);

        entityManager.addComponent<DamageComponent>(monsterEntity,
                                                    DamageComponent{0});

        sendEntitySpawn(monsterEntity);

        lastMonsterSpawnTime = now;
        monstersSpawned++;
    } else if (!bossSpawned && noActiveMonsters()) {
        spawnBoss3(posX, posY, velocityX, velocityY);
    } else if (bossSpawned) {
        spawnBoss3Caracteristic(posX, posY, velocityX, velocityY, now);
    }
}

void Server::Spawnstage4(std::chrono::steady_clock::time_point now) {
    float posX = SCREEN_WIDTH;
    float posY = static_cast<float>(rand() % 1000);
    float velocityX;
    float velocityY = 0.0f;
    int monsterTypesArray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int monsterType = monsterTypesArray[rand() % 9];
    if (monsterType == 0 || monsterType == 3 || monsterType == 7) {
        velocityX = MONSTER_SPEED_X * 4.5f;
    } else if (monsterType == 1 || monsterType == 4 || monsterType == 5 ||
               monsterType == 6) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    } else if (monsterType == 2) {
        velocityX = MONSTER_SPEED_X * 2.5f;
    } else if (monsterType == 8) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    }

    if (monstersSpawned < monstersToSpawn - 1) {
        Entity monsterEntity = entityManager.create_entity();
        entityManager.addComponent<TypeComponent>(
            monsterEntity, TypeComponent{EntityType::Monster});
        entityManager.addComponent<VelocityComponent>(
            monsterEntity,
            VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
        entityManager.addComponent<OwnerComponent>(monsterEntity,
                                                   OwnerComponent{0});
        entityManager.addComponent<MonsterType>(monsterEntity,
                                                MonsterType{monsterType});

        BehaviorComponent behavior;
        SizeComponent size;

        switch (monsterType) {
        case 0:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Zigzag;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{2 + difficulty, 2 + difficulty});
            size.width = 82.5f;
            size.height = 90.0f;
            break;
        case 1:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::StraightLine;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 162.5f;
            size.height = 125.0f;
            break;
        case 2:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{1 + difficulty, 1 + difficulty});
            size.width = 85.0f;
            size.height = 82.5f;
            break;
        case 3:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, 931.5f});
            behavior.movementPattern = MovementPattern::StraightLineSpeed;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{3 + difficulty, 3 + difficulty});
            size.width = 148.5f;
            size.height = 148.5f;
            break;
        case 4:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::RandomBounce;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 115.5f;
            size.height = 115.5f;
            break;
        case 5:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::RandomWalk;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 99.0f;
            size.height = 88.0f;
            break;
        case 6:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Hover;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 82.5f;
            size.height = 82.5f;
            break;
        case 7:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, 1026.0f});
            behavior.movementPattern = MovementPattern::StraightLineSpeed;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 75.0f;
            size.height = 75.0f;
            break;
        case 8:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{6 + difficulty, 6 + difficulty});
            size.width = 181.5f;
            size.height = 181.5f;
            break;

        default:
            break;
        }
        entityManager.addComponent<BehaviorComponent>(monsterEntity, behavior);
        entityManager.addComponent<SizeComponent>(monsterEntity, size);
        entityManager.addComponent<DamageComponent>(monsterEntity,
                                                    DamageComponent{0});
        sendEntitySpawn(monsterEntity);
        lastMonsterSpawnTime = now;
        monstersSpawned++;
    } else if (!bossSpawned && noActiveMonsters()) {
        spawnBoss4(posX, posY, velocityX, velocityY);
    } else if (bossSpawned) {
        spawnBoss4Caracteristic(posX, posY, velocityX, velocityY, now);
    }
}

void Server::SpawnstageOther(std::chrono::steady_clock::time_point now,
                             int bossToSpawn) {
    float posX = SCREEN_WIDTH;
    float posY = static_cast<float>(rand() % 1000);
    int monsterTypesArray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int monsterType = monsterTypesArray[rand() % 9];
    int bossTypesArray[] = {10, 20, 30, 40};
    int bossType = bossTypesArray[rand() % 4];

    float velocityX;
    float velocityY = 0.0f;

    if (monsterType == 0 || monsterType == 3 || monsterType == 7) {
        velocityX = MONSTER_SPEED_X * 4.5f;
    } else if (monsterType == 1 || monsterType == 4 || monsterType == 5 ||
               monsterType == 6) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    } else if (monsterType == 2) {
        velocityX = MONSTER_SPEED_X * 2.5f;
    } else if (monsterType == 8) {
        velocityX = MONSTER_SPEED_X * 5.0f;
    }

    if (monstersSpawned < monstersToSpawn - 1) {
        Entity monsterEntity = entityManager.create_entity();
        entityManager.addComponent<TypeComponent>(
            monsterEntity, TypeComponent{EntityType::Monster});
        entityManager.addComponent<VelocityComponent>(
            monsterEntity,
            VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
        entityManager.addComponent<OwnerComponent>(monsterEntity,
                                                   OwnerComponent{0});
        entityManager.addComponent<MonsterType>(monsterEntity,
                                                MonsterType{monsterType});

        BehaviorComponent behavior;
        SizeComponent size;

        switch (monsterType) {
        case 0:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Zigzag;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{2 + difficulty, 2 + difficulty});
            size.width = 82.5f;
            size.height = 90.0f;
            break;
        case 1:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::StraightLine;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 162.5f;
            size.height = 125.0f;
            break;
        case 2:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{1 + difficulty, 1 + difficulty});
            size.width = 85.0f;
            size.height = 82.5f;
            break;
        case 3:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, 931.5f});
            behavior.movementPattern = MovementPattern::StraightLineSpeed;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{3 + difficulty, 3 + difficulty});
            size.width = 148.5f;
            size.height = 148.5f;
            break;
        case 4:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::RandomBounce;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 115.5f;
            size.height = 115.5f;
            break;
        case 5:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::RandomWalk;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 99.0f;
            size.height = 88.0f;
            break;
        case 6:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Hover;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 82.5f;
            size.height = 82.5f;
            break;
        case 7:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, 1026.0f});
            behavior.movementPattern = MovementPattern::StraightLineSpeed;
            behavior.collisionBehavior = CollisionBehavior::SelfDestruct;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{4 + difficulty, 4 + difficulty});
            size.width = 75.0f;
            size.height = 75.0f;
            break;
        case 8:
            entityManager.addComponent<PositionComponent>(
                monsterEntity, PositionComponent{posX, posY});
            behavior.movementPattern = MovementPattern::Circular;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<HealthComponent>(
                monsterEntity, HealthComponent{6 + difficulty, 6 + difficulty});
            size.width = 181.5f;
            size.height = 181.5f;
            break;

        default:
            break;
        }
        entityManager.addComponent<BehaviorComponent>(monsterEntity, behavior);
        entityManager.addComponent<SizeComponent>(monsterEntity, size);
        entityManager.addComponent<DamageComponent>(monsterEntity,
                                                    DamageComponent{0});
        sendEntitySpawn(monsterEntity);
        lastMonsterSpawnTime = now;
        monstersSpawned++;
    } else if (!bossSpawned && noActiveMonsters() && bossToSpawn) {
        newBossType = bossType;
        difficulty++;
        switch (bossType) {
        case 10:
            spawnBoss1(posX, posY, velocityX, velocityY);
            break;
        case 20:
            spawnBoss2(velocityX, velocityY);
            break;
        case 30:
            spawnBoss3(posX, posY, velocityX, velocityY);
            break;
        case 40:
            spawnBoss4(posX, posY, velocityX, velocityY);
            break;
        default:
            break;
        }
    } else if (bossSpawned && bossToSpawn) {
        switch (newBossType) {
        case 10:
            break;
        case 20:
            spawnBoss2Mignon(posX, posY, velocityX, velocityY, now);
            break;
        case 30:
            spawnBoss3Caracteristic(posX, posY, velocityX, velocityY, now);
            break;
        case 40:
            spawnBoss4Caracteristic(posX, posY, velocityX, velocityY, now);
            break;
        default:
            break;
        }
    }
}
