/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** Minigame Menu
*/

#include "minigame_menu.hpp"
#include "client.hpp"
#include "pathhelper.hpp"
#include "scenemanager.hpp"
#include "utils.hpp"
#include <iostream>

MiniGameMenu::MiniGameMenu(Client &client) : _client(client) {}

void MiniGameMenu::initialize() {

    PathHelper pathHelper;
    if (!initTexture(_backgroundSprite, _backgroundTexture,
                     "minigame/background/1/woods.png", sf::Vector2f(0, 0),
                     sf::Vector2f(1, 1))) {
        std::cerr << "Failed to load minigame background texture." << std::endl;
    }
    if (!initTexture(_backButtonSprite, _backButtonTexture,
                     "buttons/Back Button.png", sf::Vector2f(800, 500),
                     sf::Vector2f(0.5f, 0.5f))) {
        std::cerr << "Failed to load back button texture." << std::endl;
    }
    if (!initTexture(_startButtonSprite, _startButtonTexture,
                     "buttons/Start Button.png", sf::Vector2f(800, 700),
                     sf::Vector2f(0.5f, 0.5f))) {
        std::cerr << "Failed to load start button texture." << std::endl;
    }

    _cursorPosition = 0;
    _showCursor = true;
    _isSelecting = false;
}

void MiniGameMenu::render(sf::RenderWindow &window) {
    window.draw(_backgroundSprite);
    window.draw(_backButtonSprite);
    window.draw(_startButtonSprite);
}

void MiniGameMenu::handleMouseClick() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(_client.getWindow());
    sf::Vector2f mousePosF = _client.getWindow().mapPixelToCoords(mousePos);
    static bool wasPressed = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !wasPressed) {
        if (_backButtonSprite.getGlobalBounds().contains(mousePosF)) {
            _client.getSceneManager()->switchScene(SceneManager::LobbyScene);
            wasPressed = true;
            return;
        }
        if (_startButtonSprite.getGlobalBounds().contains(mousePosF)) {
            _client.getSceneManager()->switchScene(SceneManager::MiniGameScene);
            wasPressed = true;
            return;
        }
    }

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        wasPressed = false;
    }

    if (_backButtonSprite.getGlobalBounds().contains(mousePosF)) {
        _backButtonSprite.setColor(sf::Color::Red);
    } else {
        _backButtonSprite.setColor(sf::Color::White);
    }
    if (_startButtonSprite.getGlobalBounds().contains(mousePosF)) {
        _startButtonSprite.setColor(sf::Color::Red);
    } else {
        _startButtonSprite.setColor(sf::Color::White);
    }
}

void MiniGameMenu::update(float deltaTime) {
    handleMouseClick();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        _client.getSceneManager()->switchScene(SceneManager::LobbyScene);
    }
}
