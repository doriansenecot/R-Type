/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** lobby
*/

#include "lobby.hpp"
#include "client.hpp"
#include "pathhelper.hpp"
#include "scenemanager.hpp"
#include "utils.hpp"
#include <iostream>

Lobby::Lobby(Client &client) : _client(client), _settings(client) {
    // Constructor
}

void Lobby::initialize() {
    _settings.initialize();
    initTexture(_backgroundSprite, _backgroundTexture, "lobby.png",
                sf::Vector2f(0, 0), sf::Vector2f(1, 1));
    initTexture(_playButtonSprite, _playButtonTexture,
                "buttons/Play Button.png", sf::Vector2f(800, 600),
                sf::Vector2f(0.6f, 0.6f));
    initTexture(_quitButtonSprite, _quitButtonTexture,
                "buttons/Quit Button.png", sf::Vector2f(800, 800),
                sf::Vector2f(0.6f, 0.6f));
    initTexture(_paramsSprite, _paramsTexture,
                "Square Buttons/Settings Square Button.png",
                sf::Vector2f(1800, 20), sf::Vector2f(0.5f, 0.5f));
    initTexture(_TitleSprite, _TitleTexture, "titre.png",
                sf::Vector2f(425, 100), sf::Vector2f(0.3f, 0.3f));
    initTexture(_settingsOverlaySprite, _settingsOverlayTexture, "GUI.png",
                sf::Vector2f(-100, 300), sf::Vector2f(11.0f, 8.5f));
    initTexture(_miniGameButtonSprite, _miniGameButtonTexture,
                "buttons/Play Button.png", sf::Vector2f(50, 50),
                sf::Vector2f(0.5f, 0.5f));

    PathHelper pathHelper;
    if (!_clickBuffer.loadFromFile(pathHelper.getSoundPath("click.ogg"))) {
        std::cerr << "Failed to load click sound effect." << std::endl;
    }
    _clickSound.setBuffer(_clickBuffer);
}

void Lobby::update(float deltaTime) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(_client.getWindow());
    sf::Vector2f mousePosF = _client.getWindow().mapPixelToCoords(mousePos);
    static bool wasPressed = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
        !_settings.getShowSettingsOverlay()) {
        if (!wasPressed) {
            wasPressed = true;

            if (_playButtonSprite.getGlobalBounds().contains(mousePosF)) {
                _client.getSceneManager()->switchScene(
                    SceneManager::ConnectionScene);
                _clickSound.play();
            }

            if (_quitButtonSprite.getGlobalBounds().contains(mousePosF)) {
                _client.getWindow().close();
                _clickSound.play();
            }
            if (_miniGameButtonSprite.getGlobalBounds().contains(mousePosF)) {
                _client.getSceneManager()->switchScene(
                    SceneManager::MenuMiniGameScene);
                _clickSound.play();
            }
        }
    } else {
        wasPressed = false;
    }
}

void Lobby::render(sf::RenderWindow &window) {
    handleMouseHover(window);
    window.draw(_backgroundSprite);

    if (_settings.getShowSettingsOverlay()) {
        _TitleSprite.setScale(0.15f, 0.15f);
        _TitleSprite.setPosition(700, 100);
        window.draw(_settingsOverlaySprite);
        _settings.render(window);
    } else {
        _TitleSprite.setScale(0.3f, 0.3f);
        _TitleSprite.setPosition(425, 100);
        window.draw(_playButtonSprite);
        window.draw(_quitButtonSprite);
        window.draw(_miniGameButtonSprite);
    }

    window.draw(_paramsSprite);
    window.draw(_TitleSprite);
}

void Lobby::handleKeyPressed(sf::Event &event) {
    _settings.handleKeyPressed(event);
}