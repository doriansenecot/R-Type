/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** windowevent
*/

#include "windowevent.hpp"
#include "client.hpp"
#include "lobby.hpp"
#include "scenemanager.hpp"
#include "windowinit.hpp"

void WindowEvent::handleWindowEvents() {
    sf::Event event;
    while (_client._window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            _client._window.close();
        } else if (event.type == sf::Event::Resized) {
            resizeView();
        } else if (event.type == sf::Event::KeyPressed &&
                   event.key.code == sf::Keyboard::F11) {
            toggleFullscreen();
        } else if (event.type == sf::Event::KeyPressed) {
            if (_client.getSceneManager()->getCurrentScene() ==
                SceneManager::ConnectionScene) {
                _client.getSceneManager()
                    ->getConnectionMenu()
                    ->handleKeyPressed(event);
            }
        } else if (event.type == sf::Event::TextEntered) {
            if (_client.getSceneManager()->getCurrentScene() ==
                SceneManager::ConnectionScene) {
                _client.getSceneManager()->getConnectionMenu()->handleTextInput(
                    event);
            }
        }
        _client.getSceneManager()->getLobby()->handleMouseClick(
            event, _client._window);
        _client.getSceneManager()->getLobby()->handleKeyPressed(event);
    }
}

void WindowEvent::resizeView() {
    float windowWidth = static_cast<float>(_client._window.getSize().x);
    float windowHeight = static_cast<float>(_client._window.getSize().y);
    float windowAspectRatio = windowWidth / windowHeight;

    if (windowAspectRatio > _client._aspectRatio) {
        float newWidth = _client._aspectRatio * windowHeight;
        float viewportX = (windowWidth - newWidth) / (2 * windowWidth);
        _client._view.setViewport(
            sf::FloatRect(viewportX, 0.0f, newWidth / windowWidth, 1.0f));
    } else {
        float newHeight = windowWidth / _client._aspectRatio;
        float viewportY = (windowHeight - newHeight) / (2 * windowHeight);
        _client._view.setViewport(
            sf::FloatRect(0.0f, viewportY, 1.0f, newHeight / windowHeight));
    }
}

void WindowEvent::toggleFullscreen() {
    _client._isFullscreen = !_client._isFullscreen;
    _client._window.close();
    _client._windowInit->initializeWindow(_client._isFullscreen);
}
