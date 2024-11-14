/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** scenemanager
*/

#ifndef SCENEMANAGER_HPP_
#define SCENEMANAGER_HPP_

#include "client.hpp"
#include "connection_menu.hpp"
#include "game.hpp"
#include "lobby.hpp"
#include "minigame_game.hpp"
#include "minigame_menu.hpp"
#include "settings.hpp"

class SceneManager {
  public:
    enum Scene {
        LobbyScene,
        GameScene,
        MenuScene,
        ConnectionScene,
        MenuMiniGameScene,
        MiniGameScene
    };

    SceneManager(Client &client);
    void update(float deltaTime);
    Scene getCurrentScene() const { return _currentScene; }

  public:
    ConnectionMenu *getConnectionMenu() { return &_connectionMenu; }

    Lobby *getLobby() { return &_lobby; }

    Game *getGame() { return &_game; }

    Settings *getSettings() { return &_settings; }

    void render();
    void switchScene(Scene newScene);
    int getSoundLevel() const { return _soundLevel; }
    int getBrightnessLevel() const { return _brightnessLevel; }
    void setSoundLevel(int level) { _soundLevel = level; }
    void setBrightnessLevel(int level);
    void handleDisconnection(int clientId);

  private:
    Scene _currentScene;
    Client &_client;
    Lobby _lobby;
    ConnectionMenu _connectionMenu;
    Settings _settings;
    Game _game;
    MiniGameMenu _miniGameMenu;
    MiniGame _miniGame;
    int _soundLevel = 100;
    int _brightnessLevel = 100;
    sf::RectangleShape _brightnessOverlay;
};

#endif /* !SCENEMANAGER_HPP_ */
