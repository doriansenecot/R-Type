/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include "connection_protocol.hpp"
#include "ecs_commands.hpp"
#include "keymapping.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <memory>

class WindowInit;
class WindowEvent;
class SceneManager;

class Client {
  public:
    Client();
    ~Client();
    void run();
    SceneManager *getSceneManager() { return _sceneManager.get(); }
    void setAspectRatio(float aspectRatio) { _aspectRatio = aspectRatio; }
    float getAspectRatio() const { return _aspectRatio; }
    WindowEvent *getWindowEvent() { return _windowEvent.get(); }
    void setFPS(int fps) { _window.setFramerateLimit(fps); }
    void updateDeltaTime();
    sf::RenderWindow &getWindow() { return _window; }
    KeyMapping &getKeyMapping() { return keyMapping; }
    void updateAllsound();
    ConnectionProtocol *getConnectionProtocol() { return &_connectionProtocol; }
    int clientId;
    std::string playerName;
    ECSCommands ecsCommands;

  private:
    KeyMapping keyMapping;
    friend class WindowInit;
    friend class WindowEvent;
    friend class SceneManager;

    sf::RenderWindow _window;
    sf::View _view;
    sf::Texture _backgroundTexture;
    sf::Sprite _backgroundSprite;
    float _aspectRatio = 16.0f / 9.0f;
    bool _isFullscreen = false;
    sf::Clock _clock;
    sf::Music _mainTheme;
    float _deltaTime;

    std::unique_ptr<WindowInit> _windowInit;
    std::unique_ptr<WindowEvent> _windowEvent;
    std::unique_ptr<SceneManager> _sceneManager;

    ConnectionProtocol _connectionProtocol{*this};
};

#endif /* !CLIENT_HPP_ */
