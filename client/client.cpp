/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** client
*/

#include "client.hpp"
#include "pathhelper.hpp"
#include "scenemanager.hpp"
#include "windowevent.hpp"
#include "windowinit.hpp"

Client::Client()
    : _windowInit(std::make_unique<WindowInit>(*this)),
      _windowEvent(std::make_unique<WindowEvent>(*this)),
      _sceneManager(std::make_unique<SceneManager>(*this)),
      _connectionProtocol(*this) {
    PathHelper pathHelper;
    if (!_mainTheme.openFromFile(pathHelper.getSoundPath("maintheme.ogg"))) {
        std::cerr << "Error: Could not load main theme music." << std::endl;
    }
    _mainTheme.setLoop(true);
}

Client::~Client() {
    // Destructor
}

void Client::run() {
    _windowInit->initializeWindow();
    _mainTheme.play();
    while (_window.isOpen()) {
        updateDeltaTime();
        updateAllsound();
        _windowEvent->handleWindowEvents();
        ecsCommands.executeAll();
        _sceneManager->update(_deltaTime);
        _sceneManager->render();
    }
}

void Client::updateAllsound() {
    _sceneManager->getLobby()->setvolumeclick(_sceneManager->getSoundLevel());
    _mainTheme.setVolume(_sceneManager->getSoundLevel());
    _sceneManager->getGame()->setvolumelaser(_sceneManager->getSoundLevel() /
                                             2);
}

void Client::updateDeltaTime() { _deltaTime = _clock.restart().asSeconds(); }
