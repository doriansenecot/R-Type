/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** scenemanager
*/

#include "scenemanager.hpp"

SceneManager::SceneManager(Client &client)
    : _client(client), _currentScene(LobbyScene), _lobby(client),
      _connectionMenu(client), _settings(client), _game(client),
      _miniGameMenu(client), _miniGame(client), _soundLevel(100),
      _brightnessLevel(100) {
    _lobby.initialize();
    _settings.initialize();
    _connectionMenu.initialize();
    _miniGameMenu.initialize();
    _miniGame.initialize();
    _brightnessOverlay.setSize(sf::Vector2f(1920, 1080));
    _brightnessOverlay.setFillColor(sf::Color(255, 255, 255, 0));
}

void SceneManager::render() {
    _client._window.clear(sf::Color::Black);
    _client._window.setView(_client._view);

    switch (_currentScene) {
    case LobbyScene:
        _lobby.render(_client._window);
        break;
    case ConnectionScene:
        _connectionMenu.render(_client._window);
        break;
    case GameScene:
        _game.render(_client._window);
        break;
    case MenuMiniGameScene:
        _miniGameMenu.render(_client._window);
        break;
    case MiniGameScene:
        _miniGame.render(_client._window);
        break;
    case MenuScene:
        break;
    }
    _client._window.draw(_brightnessOverlay);
    _client._window.display();
}

void SceneManager::update(float deltaTime) {
    if (_currentScene == LobbyScene) {
        _lobby.update(deltaTime);
    } else if (_currentScene == ConnectionScene) {
        _connectionMenu.update(deltaTime);
    } else if (_currentScene == GameScene) {
        _game.update(deltaTime);
    } else if (_currentScene == MenuMiniGameScene) {
        _miniGameMenu.update(deltaTime);
    } else if (_currentScene == MiniGameScene) {
        _miniGameMenu.update(deltaTime);
    }
}

void SceneManager::switchScene(Scene newScene) { _currentScene = newScene; }

void SceneManager::handleDisconnection(int clientId) {
    if (_client.clientId == clientId) {
        _game.setPlayerIsDead(true);
    } else {
        if (_currentScene == GameScene) {
            _game.removeClientEntity(clientId);
        }
    }
}

void SceneManager::setBrightnessLevel(int level) {
    _brightnessLevel = std::clamp(level, 0, 100);
    int alpha = (100 - _brightnessLevel) * 255 / 100;
    _brightnessOverlay.setFillColor(sf::Color(0, 0, 0, alpha));
}
