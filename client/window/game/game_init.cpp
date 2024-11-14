/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** game_init
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

void Game::initialize() {
    stageSystem.setStage(1);
    fadeOverlay.setSize(sf::Vector2f(1920, 1080));
    fadeOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    gCoordinator.init();

    gCoordinator.registerComponent<TransformComponent>();
    gCoordinator.registerComponent<SpriteComponent>();
    gCoordinator.registerComponent<VelocityComponent>();
    gCoordinator.registerComponent<InputComponent>();
    gCoordinator.registerComponent<BulletComponent>();
    gCoordinator.registerComponent<NameComponent>();
    gCoordinator.registerComponent<TypeComponent>();
    gCoordinator.registerComponent<OwnerComponent>();
    gCoordinator.registerComponent<AnimationComponent>();
    gCoordinator.registerComponent<HealthComponent>();
    gCoordinator.registerComponent<XPComponent>();
    gCoordinator.registerComponent<LevelComponent>();
    gCoordinator.registerComponent<ScoreComponent>();

    movementSystem = gCoordinator.registerSystem<MovementSystem>();
    renderSystem = gCoordinator.registerSystem<RenderSystem>();
    inputSystem = gCoordinator.registerSystem<InputSystem>();
    inputSystem->setKeyMapping(_client.getKeyMapping());
    nameRenderSystem = gCoordinator.registerSystem<NameRenderSystem>();
    healthSystem = gCoordinator.registerSystem<HealthSystem>();
    levelSystem = gCoordinator.registerSystem<LevelSystem>();

    Signature levelSignature;
    levelSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                       true);
    levelSignature.set(gCoordinator.getComponentType<XPComponent>(), true);
    levelSignature.set(gCoordinator.getComponentType<LevelComponent>(), true);
    gCoordinator.setSystemSignature<LevelSystem>(levelSignature);

    Signature healthSignature;
    healthSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                        true);
    healthSignature.set(gCoordinator.getComponentType<HealthComponent>(), true);
    gCoordinator.setSystemSignature<HealthSystem>(healthSignature);

    Signature nameRenderSignature;
    nameRenderSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                            true);
    nameRenderSignature.set(gCoordinator.getComponentType<NameComponent>(),
                            true);
    gCoordinator.setSystemSignature<NameRenderSystem>(nameRenderSignature);

    Signature inputSignature;
    inputSignature.set(gCoordinator.getComponentType<InputComponent>(), true);
    gCoordinator.setSystemSignature<InputSystem>(inputSignature);

    Signature movementSignature;
    movementSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                          true);
    movementSignature.set(gCoordinator.getComponentType<VelocityComponent>(),
                          true);
    gCoordinator.setSystemSignature<MovementSystem>(movementSignature);

    explosionSystem = gCoordinator.registerSystem<ExplosionSystem>();

    Signature explosionSignature;
    explosionSignature.set(gCoordinator.getComponentType<SpriteComponent>(),
                           true);
    explosionSignature.set(gCoordinator.getComponentType<AnimationComponent>(),
                           true);
    gCoordinator.setSystemSignature<ExplosionSystem>(explosionSignature);

    Signature renderSignature;
    renderSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                        true);
    renderSignature.set(gCoordinator.getComponentType<SpriteComponent>(), true);
    gCoordinator.setSystemSignature<RenderSystem>(renderSignature);

    ScoreComponent score;
    score.score = 0;
    gCoordinator.addComponent(playerEntity, score);
    scoreSystem = gCoordinator.registerSystem<ScoreSystem>();

    Signature scoreSignature;
    scoreSignature.set(gCoordinator.getComponentType<ScoreComponent>(), true);
    gCoordinator.setSystemSignature<ScoreSystem>(scoreSignature);

    PathHelper pathHelper;
    if (!spaceshipTexture.loadFromFile(
            pathHelper.getImagePath("r-typesheet42.gif"))) {
        std::cerr << "Failed to load spaceship texture." << std::endl;
    }
    if (!monsterTexture1.loadFromFile(
            pathHelper.getImagePath("r-typesheet5.gif"))) {
        std::cerr << "Failed to load monster texture 1." << std::endl;
    }
    if (!monsterTexture2.loadFromFile(
            pathHelper.getImagePath("r-typesheet26.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!monsterTexture3.loadFromFile(
            pathHelper.getImagePath("r-typesheet8.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!monsterTexture4.loadFromFile(
            pathHelper.getImagePath("r-typesheet7.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!monsterTexture5.loadFromFile(
            pathHelper.getImagePath("r-typesheet23.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!monsterTexture6.loadFromFile(
            pathHelper.getImagePath("r-typesheet16.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!monsterTexture7.loadFromFile(
            pathHelper.getImagePath("r-typesheet14.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!monsterTexture8.loadFromFile(
            pathHelper.getImagePath("r-typesheet18.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!monsterTexture9.loadFromFile(
            pathHelper.getImagePath("r-typesheet22.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!projectileTexturemonster.loadFromFile(
            pathHelper.getImagePath("r-typesheet30a.gif"))) {
        std::cerr << "Failed to load missile texture." << std::endl;
    }
    if (!explosionTexture.loadFromFile(
            pathHelper.getImagePath("r-typesheet44.gif"))) {
        std::cerr << "Failed to load explosion texture." << std::endl;
    }
    if (!boss1.loadFromFile(pathHelper.getImagePath("r-typesheet17.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!boss2.loadFromFile(pathHelper.getImagePath("r-typesheet36.gif"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!boss3.loadFromFile(pathHelper.getImagePath("r-typesheet50.png"))) {
        std::cerr << "Failed to load monster texture 2." << std::endl;
    }
    if (!finalboss.loadFromFile(pathHelper.getImagePath("r-typesheet51.png"))) {
        std::cerr << "Failed to load missile texture." << std::endl;
    }
    if (!projectileTexture.loadFromFile(
            pathHelper.getImagePath("r-typesheet1.gif"))) {
        std::cerr << "Failed to load missile texture." << std::endl;
    }
    if (!powerUpTexture.loadFromFile(
            pathHelper.getImagePath("r-typesheet3.gif"))) {
        std::cerr << "Failed to load power-up texture." << std::endl;
    }
    if (!_quitButtonTexture.loadFromFile(
            pathHelper.getImagePath("buttons/Quit Button.png"))) {
        std::cerr << "Failed to load Quit Button texture." << std::endl;
    }

    playerEntity = gCoordinator.createEntity();
    auto rect = getTextureRectForClientID(_client.clientId);
    setupSpaceship(playerEntity, sf::Vector2f(400, 540), rect, "Player");

    const Stage &initialStage = stageSystem.getCurrentStageData();
    if (currentBackgroundTexture.loadFromFile(initialStage.backgroundPath)) {
        currentBackgroundSprite.setTexture(currentBackgroundTexture);
    }
    _backgroundSize = currentBackgroundTexture.getSize();
    _gameView.setSize(1920, 1080);

    if (!font.loadFromFile(pathHelper.getFontPath("SHUTTLE-X.ttf"))) {
        std::cerr << "Failed to load font." << std::endl;
    }
    levelText.setFont(font);
    levelText.setCharacterSize(36);
    levelText.setFillColor(sf::Color::White);
    levelText.setStyle(sf::Text::Bold);
    laserSystem = gCoordinator.registerSystem<LaserSystem>();
    laserSystem->setProjectileTexture(&projectileTexture);
    Signature laserSignature;
    laserSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                       true);
    laserSignature.set(gCoordinator.getComponentType<SpriteComponent>(), true);
    laserSignature.set(gCoordinator.getComponentType<BulletComponent>(), true);
    gCoordinator.setSystemSignature<LaserSystem>(laserSignature);

    if (!_laserBuffer.loadFromFile(pathHelper.getSoundPath("shoot.ogg"))) {
        std::cerr << "Failed to load click sound effect." << std::endl;
    }
    _laserSound.setBuffer(_laserBuffer);
    if (!_explosionBuffer.loadFromFile(
            pathHelper.getSoundPath("explosion.ogg"))) {
        std::cerr << "Failed to load monster destroy sound effect."
                  << std::endl;
    }
    _explosionSound.setBuffer(_explosionBuffer);
    setupWaitingArea();
    waitingOverlay.setSize(sf::Vector2f(1920, 1080));
    waitingOverlay.setFillColor(sf::Color(0, 0, 0, 150));

    waitingText.setFont(font);
    waitingText.setString("Waiting for all players to join...\n\tPlease go to "
                          "the waiting area if all players have joined.");
    waitingText.setCharacterSize(50);
    waitingText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = waitingText.getLocalBounds();
    waitingText.setOrigin(textRect.left + textRect.width / 2.0f,
                          textRect.top + textRect.height / 2.0f);
    waitingText.setPosition(1920 / 2.0f, 1080 / 1.2f);
    stageText.setFont(font);
    stageText.setCharacterSize(100);
    stageText.setFillColor(sf::Color::White);
    stageText.setOutlineColor(sf::Color::Black);
    stageText.setOutlineThickness(5);
    stageText.setStyle(sf::Text::Bold);
    stageText.setPosition(960, 540);
    stageText.setString("");
    setupLoadingCircle();

    _quitButtonSprite.setTexture(_quitButtonTexture);
    _quitButtonSprite.setPosition(1920 / 2.0f - 150, 1080 / 2.0f - 50);
    _quitButtonSprite.setScale(0.5f, 0.5f);

    _isQuitButtonVisible = false;
}

void Game::initializeCommonComponents(Entity newEntity, float posX, float posY,
                                      float scaleX, float scaleY,
                                      float velocityX, float velocityY) {
    TransformComponent transform;
    transform.position = sf::Vector2f(posX, posY);
    transform.scale = sf::Vector2f(scaleX, scaleY);
    transform.rotation = 0.0f;
    gCoordinator.addComponent(newEntity, transform);

    VelocityComponent velocity;
    velocity.velocity = sf::Vector2f(velocityX, velocityY);
    gCoordinator.addComponent(newEntity, velocity);
}

void Game::initializeMonster(Entity newEntity, float posX, float posY,
                             float velocityX, float velocityY,
                             int monsterType) {
    TransformComponent transform;
    transform.position = sf::Vector2f(posX, posY);
    transform.rotation = 0.0f;
    VelocityComponent velocity;
    velocity.velocity = sf::Vector2f(velocityX, velocityY);
    SpriteComponent sprite;
    AnimationComponent animation;
    animation.frameTime = 0.1f;
    switch (monsterType) {
    case 0:
        transform.scale = sf::Vector2f(2.5f, 2.5f);
        sprite.sprite.setTexture(monsterTexture1);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 33, 36));
        animation.numFrames = 8;
        break;
    case 1:
        transform.scale = sf::Vector2f(2.5f, 2.5f);
        sprite.sprite.setTexture(monsterTexture2);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 65, 50));
        animation.numFrames = 3;
        break;
    case 2:
        transform.scale = sf::Vector2f(2.5f, 2.5f);
        sprite.sprite.setTexture(monsterTexture3);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 33, 34));
        animation.numFrames = 16;
        break;
    case 3:
        transform.scale = sf::Vector2f(4.5f, 4.5f);
        sprite.sprite.setTexture(monsterTexture4);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 33, 33));
        animation.numFrames = 6;
        break;
    case 4:
        transform.scale = sf::Vector2f(3.5f, 3.5f);
        sprite.sprite.setTexture(monsterTexture5);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 33, 33));
        animation.numFrames = 8;
        break;
    case 5:
        transform.scale = sf::Vector2f(5.5f, 5.5f);
        sprite.sprite.setTexture(monsterTexture6);
        sprite.sprite.setTextureRect(sf::IntRect(272, 8, 17, 16));
        animation.numFrames = 4;
        break;
    case 6:
        transform.scale = sf::Vector2f(2.5f, 2.5f);
        sprite.sprite.setTexture(monsterTexture8);
        sprite.sprite.setTextureRect(sf::IntRect(0, 66, 33, 33));
        animation.numFrames = 4;
        break;
    case 7:
        transform.scale = sf::Vector2f(1.5f, 1.5f);
        sprite.sprite.setTexture(monsterTexture7);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
        animation.numFrames = 5;
        break;
    case 8:
        transform.scale = sf::Vector2f(5.5f, 5.5f);
        sprite.sprite.setTexture(monsterTexture9);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 33, 33));
        animation.numFrames = 16;
        break;
    case 10:
        transform.scale = sf::Vector2f(2.5f, 2.5f);
        sprite.sprite.setTexture(boss1);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 65, 132));
        animation.numFrames = 8;
        break;
    case 20:
        transform.scale = sf::Vector2f(4.0f, 4.0f);
        sprite.sprite.setTexture(boss2);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 264, 143));
        animation.numFrames = 8;
        break;
    case 30:
        transform.scale = sf::Vector2f(4.5f, 4.5f);
        sprite.sprite.setTexture(boss3);
        sprite.sprite.setTextureRect(sf::IntRect(42, 278, 100, 115));
        animation.numFrames = 13;
        break;
    case 31:
        transform.scale = sf::Vector2f(2.5f, 2.5f);
        sprite.sprite.setTexture(projectileTexturemonster);
        sprite.sprite.setColor(sf::Color::Red);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 34, 34));
        animation.numFrames = 3;
        break;
    case 40:
        transform.scale = sf::Vector2f(3.5f, 3.5f);
        sprite.sprite.setTexture(finalboss);
        sprite.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        animation.numFrames = 6;
        break;
    default:
        std::cerr << "Unknown monster type: " << monsterType << std::endl;
        return;
    }

    animation.monsterType = monsterType;
    gCoordinator.addComponent(newEntity, transform);
    gCoordinator.addComponent(newEntity, velocity);
    gCoordinator.addComponent(newEntity, sprite);
    gCoordinator.addComponent(newEntity, animation);
}

void Game::initializeMissile(Entity newEntity, float posX, float posY) {
    initializeCommonComponents(newEntity, posX + 40, posY + 5, 3.0f, 3.0f, 0.0f,
                               0.0f);

    SpriteComponent sprite;
    sprite.sprite.setTexture(projectileTexture);
    sprite.sprite.setTextureRect(sf::IntRect(246, 80, 18, 18));
    gCoordinator.addComponent(newEntity, sprite);
}

void Game::initializePowerUp(Entity newEntity, float posX, float posY) {
    initializeCommonComponents(newEntity, posX, posY, 3.0f, 3.0f, 0.0f, 0.0f);

    SpriteComponent sprite;
    sprite.sprite.setTexture(powerUpTexture);
    sprite.sprite.setTextureRect(sf::IntRect(0, 0, 17, 18));
    gCoordinator.addComponent(newEntity, sprite);

    AnimationComponent animation;
    animation.frameIndex = 0;
    animation.frameTime = 0.1f;
    animation.numFrames = 12;
    gCoordinator.addComponent(newEntity, animation);
}
