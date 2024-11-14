/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** connection_menu.hpp
*/

#ifndef CONNECTION_MENU_HPP_
#define CONNECTION_MENU_HPP_

#include "client.hpp"
#include "settings.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Clipboard.hpp>

class ConnectionMenu {
  public:
    ConnectionMenu(Client &client);
    ~ConnectionMenu();

    void initialize();
    void render(sf::RenderWindow &window);
    void handleMouseClick();
    void handleTextInput(sf::Event event);
    void handleKeyPressed(sf::Event event);
    void update(float deltaTime);

  private:
    void initInputField(sf::Text &title, sf::Text &textField,
                        sf::RectangleShape &box, const std::string &placeholder,
                        const std::string &labelText, sf::Vector2f position);

    Client &_client;
    Settings _settings;

    sf::Texture _backgroundTexture;
    sf::Sprite _backgroundSprite;

    sf::Texture _paramsmenuTexture;
    sf::Sprite _paramsmenuSprite;

    sf::Texture _overlayTexture;
    sf::Sprite _overlaySprite;

    sf::Texture _TitleTexture;
    sf::Sprite _TitleSprite;

    sf::Font _font;
    sf::Text _nameTitle;
    sf::Text _nameField;
    sf::Text _ipTitle;
    sf::Text _ipField;
    sf::Text _portTitle;
    sf::Text _portField;

    sf::RectangleShape _nameBox;
    sf::RectangleShape _ipBox;
    sf::RectangleShape _portBox;

    sf::Text *_activeField;
    size_t _cursorPosition;
    bool _isSelecting;
    bool _showCursor;
    sf::Clock _cursorBlinkClock;

    sf::Texture _backButtonTexture;
    sf::Sprite _backButtonSprite;
    sf::Texture _startButtonTexture;
    sf::Sprite _startButtonSprite;

    sf::SoundBuffer _clickBuffer;
    sf::Sound _clickSound;
};

#endif /* !CONNECTION_MENU_HPP_ */
