/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** lobby
*/

#ifndef LOBBY_HPP_
#define LOBBY_HPP_

#include "client.hpp"
#include "settings.hpp"
#include <SFML/Graphics.hpp>

class Lobby {
  public:
    Lobby(Client &client);
    void initialize();
    void render(sf::RenderWindow &window);
    void handleMouseHover(sf::RenderWindow &window);
    void handleButtonSpriteHover(sf::RenderWindow &window,
                                 sf::Sprite &buttonSprite);
    void handleMouseClick(sf::Event &event, sf::RenderWindow &window);
    void handleButtonHover(sf::RenderWindow &window,
                           sf::RectangleShape &buttonShape);
    void update(float deltaTime);
    void setvolumeclick(float volume) { _clickSound.setVolume(volume); }
    void handleKeyPressed(sf::Event &event);

  private:
    Client &_client;
    Settings _settings;
    sf::Sprite _backgroundSprite, _playButtonSprite, _quitButtonSprite,
        _paramsSprite, _TitleSprite, _settingsOverlaySprite,
        _miniGameButtonSprite;
    sf::Texture _backgroundTexture, _playButtonTexture, _quitButtonTexture,
        _paramsTexture, _TitleTexture, _settingsOverlayTexture,
        _miniGameButtonTexture;
    sf::SoundBuffer _clickBuffer;
    sf::Sound _clickSound;
};

#endif /* !LOBBY_HPP_ */
