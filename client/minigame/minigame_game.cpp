#include "minigame_game.hpp"
#include "client.hpp"
#include "coordinator.hpp"
#include "pathhelper.hpp"
#include "scenemanager.hpp"
#include "utils.hpp"
#include <iostream>

MiniGame::MiniGame(Client &client) : _client(client) {}

void MiniGame::initialize() {
    gCoordinator.init();

    gCoordinator.registerComponent<TransformComponent>();
    gCoordinator.registerComponent<SpriteComponent>();
    gCoordinator.registerComponent<VelocityComponent>();
    gCoordinator.registerComponent<InputComponent>();

    _backgroundSize = _backgroundTexture.getSize();
    PathHelper pathHelper;

    if (!initTexture(_backgroundSprite, _backgroundTexture,
                     "minigame/background/2/2304x1296.png", sf::Vector2f(0, 0),
                     sf::Vector2f(1, 1))) {
    } else {
    }

    std::string playerTexturePath = "minigame/archer/walk/Elf_01__WALK_000.png";
    if (!initTexture(_playerSprite, _playerTexture, playerTexturePath,
                     sf::Vector2f(100, 500), sf::Vector2f(1, 1))) {
        std::cerr << "[ERROR] Failed to load player texture: "
                  << playerTexturePath << std::endl;
    }
    playerEntity = gCoordinator.createEntity();

    sf::IntRect playerTextureRect(0, 0, 33, 17);
    setupCharacter(playerEntity,
                   sf::Vector2f(_client.getWindow().getSize().x / 2,
                                _client.getWindow().getSize().y / 2),
                   playerTextureRect);

    gCoordinator.addComponent(playerEntity, InputComponent{});
    movementSystem = gCoordinator.registerSystem<MovementSystem>();
    renderSystem = gCoordinator.registerSystem<RenderSystem>();

    Signature movementSignature;
    movementSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                          true);
    movementSignature.set(gCoordinator.getComponentType<VelocityComponent>(),
                          true);
    gCoordinator.setSystemSignature<MovementSystem>(movementSignature);

    Signature renderSignature;
    renderSignature.set(gCoordinator.getComponentType<SpriteComponent>(), true);
    renderSignature.set(gCoordinator.getComponentType<TransformComponent>(),
                        true);
    gCoordinator.setSystemSignature<RenderSystem>(renderSignature);

    _client.getWindow().setView(
        sf::View(sf::FloatRect(0, 0, _client.getWindow().getSize().x,
                               _client.getWindow().getSize().y)));
}

void MiniGame::setupCharacter(Entity entity, sf::Vector2f position,
                              sf::IntRect textureRect) {

    TransformComponent transform;
    transform.position = position;
    transform.scale = sf::Vector2f(1.5f, 1.5f);
    transform.rotation = 0.0f;
    gCoordinator.addComponent(entity, transform);

    SpriteComponent sprite;
    sprite.sprite.setTexture(_playerTexture);
    sprite.sprite.setTextureRect(textureRect);
    gCoordinator.addComponent(entity, sprite);

    VelocityComponent velocity;
    velocity.velocity = sf::Vector2f(0.0f, 0.0f);
    gCoordinator.addComponent(entity, velocity);
}

void MiniGame::render(sf::RenderWindow &window) {
    sf::Vector2f viewCenter = _client.getWindow().getView().getCenter();
    sf::Vector2f viewSize = _client.getWindow().getView().getSize();
    float viewLeftEdge = viewCenter.x - (viewSize.x / 2);

    _backgroundSprite.setPosition(viewLeftEdge - _backgroundSize.x, 0);
    window.draw(_backgroundSprite);
    _backgroundSprite.setPosition(viewLeftEdge, 0);
    window.draw(_backgroundSprite);
    _backgroundSprite.setPosition(viewLeftEdge + _backgroundSize.x, 0);
    window.draw(_backgroundSprite);

    if (renderSystem) {
        renderSystem->render(window);
    } else {
    }
}

void MiniGame::handleMouseClick() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(_client.getWindow());
    sf::Vector2f mousePosF = _client.getWindow().mapPixelToCoords(mousePos);
    static bool wasPressed = false;
}

void MiniGame::update(float deltaTime) {
    handleMouseClick();

    if (movementSystem) {
        movementSystem->update(deltaTime);
    } else {
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        _client.getSceneManager()->switchScene(SceneManager::LobbyScene);
    }
}
