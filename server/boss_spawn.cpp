/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** boss_spawn.cpp
*/

#include "server.hpp"

void Server::spawnBoss1(float posX, float posY, float velocityX,
                        float velocityY) {
    Entity bossEntity = entityManager.create_entity();
    entityManager.addComponent<TypeComponent>(
        bossEntity, TypeComponent{EntityType::Monster});
    entityManager.addComponent<MonsterType>(bossEntity, MonsterType{10});
    entityManager.addComponent<PositionComponent>(
        bossEntity, PositionComponent{posX, posY});
    entityManager.addComponent<VelocityComponent>(
        bossEntity,
        VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
    entityManager.addComponent<OwnerComponent>(bossEntity, OwnerComponent{0});
    entityManager.addComponent<HealthComponent>(
        bossEntity,
        HealthComponent{50 + difficulty * 20, 50 + difficulty * 20});
    BehaviorComponent behavior;
    behavior.movementPattern = MovementPattern::Boss;
    behavior.collisionBehavior = CollisionBehavior::Stop;
    entityManager.addComponent<BehaviorComponent>(bossEntity, behavior);
    SizeComponent size;
    size.width = 160.0f;
    size.height = 330.0f;
    entityManager.addComponent<SizeComponent>(bossEntity, size);
    entityManager.addComponent<DamageComponent>(bossEntity, DamageComponent{0});
    sendEntitySpawn(bossEntity);
    bossSpawned = true;
}

void Server::spawnBoss2(float velocityX, float velocityY) {
    Entity bossEntity = entityManager.create_entity();
    entityManager.addComponent<TypeComponent>(
        bossEntity, TypeComponent{EntityType::Monster});
    entityManager.addComponent<MonsterType>(bossEntity, MonsterType{20});
    entityManager.addComponent<PositionComponent>(
        bossEntity, PositionComponent{864.0f, 520.0f});
    entityManager.addComponent<VelocityComponent>(
        bossEntity,
        VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
    entityManager.addComponent<OwnerComponent>(bossEntity, OwnerComponent{0});
    entityManager.addComponent<HealthComponent>(
        bossEntity,
        HealthComponent{250 + difficulty * 30, 250 + difficulty * 30});
    BehaviorComponent behavior;
    behavior.movementPattern = MovementPattern::BossGround;
    behavior.collisionBehavior = CollisionBehavior::Stop;
    entityManager.addComponent<BehaviorComponent>(bossEntity, behavior);
    SizeComponent size;
    size.width = 1056.0f;
    size.height = 572.0f;
    entityManager.addComponent<SizeComponent>(bossEntity, size);
    entityManager.addComponent<DamageComponent>(bossEntity, DamageComponent{0});
    sendEntitySpawn(bossEntity);
    bossSpawned = true;
}

void Server::spawnBoss2Mignon(float posX, float posY, float velocityX,
                              float velocityY,
                              std::chrono::steady_clock::time_point now) {
    posY = static_cast<float>(rand() % 520);
    Entity mignonentity = entityManager.create_entity();
    entityManager.addComponent<TypeComponent>(
        mignonentity, TypeComponent{EntityType::Monster});
    entityManager.addComponent<MonsterType>(mignonentity, MonsterType{1});
    entityManager.addComponent<PositionComponent>(
        mignonentity, PositionComponent{posX, posY});
    entityManager.addComponent<VelocityComponent>(
        mignonentity,
        VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
    entityManager.addComponent<OwnerComponent>(mignonentity, OwnerComponent{0});
    entityManager.addComponent<HealthComponent>(
        mignonentity, HealthComponent{2 + difficulty, 2 + difficulty});
    BehaviorComponent behavior;
    behavior.movementPattern = MovementPattern::StraightLineSpeed;
    behavior.collisionBehavior = CollisionBehavior::Stop;
    entityManager.addComponent<BehaviorComponent>(mignonentity, behavior);
    SizeComponent size;
    size.width = 162.5f;
    size.height = 125.0f;
    entityManager.addComponent<SizeComponent>(mignonentity, size);
    entityManager.addComponent<DamageComponent>(mignonentity,
                                                DamageComponent{0});
    sendEntitySpawn(mignonentity);
    lastMonsterSpawnTime = now;
}

void Server::spawnBoss3(float posX, float posY, float velocityX,
                        float velocityY) {
    Entity bossEntity = entityManager.create_entity();
    entityManager.addComponent<TypeComponent>(
        bossEntity, TypeComponent{EntityType::Monster});
    entityManager.addComponent<MonsterType>(bossEntity, MonsterType{30});
    entityManager.addComponent<PositionComponent>(
        bossEntity, PositionComponent{posX, posY});
    entityManager.addComponent<VelocityComponent>(
        bossEntity,
        VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
    entityManager.addComponent<OwnerComponent>(bossEntity, OwnerComponent{0});
    entityManager.addComponent<HealthComponent>(
        bossEntity,
        HealthComponent{200 + difficulty * 20, 200 + difficulty * 20});
    BehaviorComponent behavior;
    behavior.movementPattern = MovementPattern::Boss2;
    behavior.collisionBehavior = CollisionBehavior::Stop;
    entityManager.addComponent<BehaviorComponent>(bossEntity, behavior);
    SizeComponent size;
    size.width = 234.0f;
    size.height = 351.0f;
    entityManager.addComponent<SizeComponent>(bossEntity, size);
    entityManager.addComponent<DamageComponent>(bossEntity, DamageComponent{0});
    sendEntitySpawn(bossEntity);
    bossSpawned = true;
}

void Server::spawnBoss3Caracteristic(
    float posX, float posY, float velocityX, float velocityY,
    std::chrono::steady_clock::time_point now) {
    Entity bossEntity = -1;
    for (const auto &[entity, type] :
         entityManager.getComponentMap<TypeComponent>()) {
        if (type.type == EntityType::Monster) {
            MonsterType &monsterType =
                entityManager.getComponent<MonsterType>(entity);
            if (monsterType.type == 30) {
                bossEntity = entity;
                break;
            }
        }
    }
    if (bossEntity != -1 &&
        entityManager.hasComponent<PositionComponent>(bossEntity)) {
        PositionComponent &bossPos =
            entityManager.getComponent<PositionComponent>(bossEntity);
        for (int i = 0; i < 6; ++i) {
            float offsetX = static_cast<float>(rand() % 600 - 200);
            float offsetY = static_cast<float>(rand() % 600 - 200);
            Entity mignonentity = entityManager.create_entity();
            entityManager.addComponent<TypeComponent>(
                mignonentity, TypeComponent{EntityType::Monster});
            entityManager.addComponent<MonsterType>(mignonentity,
                                                    MonsterType{31});
            entityManager.addComponent<PositionComponent>(
                mignonentity,
                PositionComponent{bossPos.x + offsetX, bossPos.y + offsetY});
            entityManager.addComponent<VelocityComponent>(
                mignonentity,
                VelocityComponent{sf::Vector2<float>(-MONSTER_SPEED_X, 0)});
            entityManager.addComponent<OwnerComponent>(mignonentity,
                                                       OwnerComponent{0});
            entityManager.addComponent<HealthComponent>(mignonentity,
                                                        HealthComponent{1, 1});
            BehaviorComponent behavior;
            behavior.movementPattern = MovementPattern::StraightLineSpeed;
            behavior.collisionBehavior = CollisionBehavior::Bounce;
            entityManager.addComponent<BehaviorComponent>(mignonentity,
                                                          behavior);
            SizeComponent size;
            size.width = 85.0f;
            size.height = 85.0f;
            entityManager.addComponent<SizeComponent>(mignonentity, size);
            entityManager.addComponent<DamageComponent>(mignonentity,
                                                        DamageComponent{0});
            sendEntitySpawn(mignonentity);
        }
        lastMonsterSpawnTime = now;
    }
}

void Server::spawnBoss4(float posX, float posY, float velocityX,
                        float velocityY) {
    Entity bossEntity = entityManager.create_entity();
    entityManager.addComponent<TypeComponent>(
        bossEntity, TypeComponent{EntityType::Monster});
    entityManager.addComponent<MonsterType>(bossEntity, MonsterType{40});
    entityManager.addComponent<PositionComponent>(
        bossEntity, PositionComponent{posX, posY});
    entityManager.addComponent<VelocityComponent>(
        bossEntity,
        VelocityComponent{sf::Vector2<float>(velocityX, velocityY)});
    entityManager.addComponent<OwnerComponent>(bossEntity, OwnerComponent{0});
    entityManager.addComponent<HealthComponent>(
        bossEntity,
        HealthComponent{400 + difficulty * 40, 400 + difficulty * 40});
    BehaviorComponent behavior;
    behavior.movementPattern = MovementPattern::Boss3;
    behavior.collisionBehavior = CollisionBehavior::Stop;
    entityManager.addComponent<BehaviorComponent>(bossEntity, behavior);
    SizeComponent size;
    size.width = 224.0f;
    size.height = 224.0f;
    entityManager.addComponent<SizeComponent>(bossEntity, size);
    entityManager.addComponent<DamageComponent>(bossEntity, DamageComponent{0});
    sendEntitySpawn(bossEntity);
    bossSpawned = true;
}

void Server::spawnBoss4Caracteristic(
    float posX, float posY, float velocityX, float velocityY,
    std::chrono::steady_clock::time_point now) {
    Entity bossEntity = -1;
    for (const auto &[entity, type] :
         entityManager.getComponentMap<TypeComponent>()) {
        if (type.type == EntityType::Monster) {
            MonsterType &monsterType =
                entityManager.getComponent<MonsterType>(entity);
            if (monsterType.type == 40) {
                bossEntity = entity;
                break;
            }
        }
    }
    if (bossEntity != -1 &&
        entityManager.hasComponent<HealthComponent>(bossEntity)) {
        HealthComponent &bossHealth =
            entityManager.getComponent<HealthComponent>(bossEntity);
        if (bossHealth.currentHealth <= bossHealth.maxHealth * 0.75f) {
            PositionComponent &bossPos =
                entityManager.getComponent<PositionComponent>(bossEntity);
            for (int i = 0; i < 10; ++i) {
                float offsetX = static_cast<float>(rand() % 600 - 200);
                float offsetY = static_cast<float>(rand() % 600 - 200);
                Entity minionEntity = entityManager.create_entity();
                entityManager.addComponent<TypeComponent>(
                    minionEntity, TypeComponent{EntityType::Monster});
                entityManager.addComponent<MonsterType>(minionEntity,
                                                        MonsterType{31});
                entityManager.addComponent<PositionComponent>(
                    minionEntity, PositionComponent{bossPos.x + offsetX,
                                                    bossPos.y + offsetY});
                entityManager.addComponent<VelocityComponent>(
                    minionEntity,
                    VelocityComponent{sf::Vector2<float>(-MONSTER_SPEED_X, 0)});
                entityManager.addComponent<OwnerComponent>(minionEntity,
                                                           OwnerComponent{0});
                if (bossHealth.currentHealth <= bossHealth.maxHealth * 0.25f)
                    entityManager.addComponent<HealthComponent>(
                        minionEntity, HealthComponent{difficulty, difficulty});
                else
                    entityManager.addComponent<HealthComponent>(
                        minionEntity, HealthComponent{1, 1});
                BehaviorComponent behavior;
                behavior.movementPattern = MovementPattern::StraightLineSpeed;
                behavior.collisionBehavior = CollisionBehavior::Bounce;
                entityManager.addComponent<BehaviorComponent>(minionEntity,
                                                              behavior);
                SizeComponent size;
                size.width = 85.0f;
                size.height = 85.0f;
                entityManager.addComponent<SizeComponent>(minionEntity, size);
                entityManager.addComponent<DamageComponent>(minionEntity,
                                                            DamageComponent{0});
                sendEntitySpawn(minionEntity);
            }
            lastMonsterSpawnTime = now;
        }
    }
}
