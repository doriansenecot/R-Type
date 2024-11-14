/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** connection_menu
*/

#include "connection_menu.hpp"
#include "connection_protocol.hpp"
#include "pathhelper.hpp"
#include "scenemanager.hpp"
#include "utils.hpp"

ConnectionMenu::ConnectionMenu(Client &client)
    : _client(client), _activeField(nullptr), _settings(client) {
    // Constructor
}

ConnectionMenu::~ConnectionMenu() {
    // Destructor
}

void ConnectionMenu::initialize() {
    _settings.initialize();
    initTexture(_backgroundSprite, _backgroundTexture, "lobby.png",
                sf::Vector2f(0, 0), sf::Vector2f(1, 1));
    initTexture(_paramsmenuSprite, _paramsmenuTexture,
                "Square Buttons/Settings Square Button.png",
                sf::Vector2f(1800, 20), sf::Vector2f(0.5f, 0.5f));
    initTexture(_overlaySprite, _overlayTexture, "GUI.png",
                sf::Vector2f(-100, 300), sf::Vector2f(11.0f, 8.5f));
    initTexture(_TitleSprite, _TitleTexture, "titre.png",
                sf::Vector2f(700, 100), sf::Vector2f(0.15f, 0.15f));
    PathHelper pathHelper;
    if (!_font.loadFromFile(pathHelper.getFontPath("SHUTTLE-X.ttf"))) {
        std::cerr << "Error loading font: SHUTTLE-X.ttf" << std::endl;
    }

    initInputField(_nameTitle, _nameField, _nameBox, "", "Enter Name",
                   sf::Vector2f(600, 200));
    initInputField(_ipTitle, _ipField, _ipBox, "", "Enter IP",
                   sf::Vector2f(600, 380));
    initInputField(_portTitle, _portField, _portBox, "", "Enter Port",
                   sf::Vector2f(600, 560));

    if (!initTexture(_backButtonSprite, _backButtonTexture,
                     "buttons/Back Button.png", sf::Vector2f(20, 20),
                     sf::Vector2f(0.6f, 0.6f))) {
        std::cerr << "Failed to load back button texture." << std::endl;
    }

    if (!initTexture(_startButtonSprite, _startButtonTexture,
                     "buttons/Start Button.png", sf::Vector2f(800, 800),
                     sf::Vector2f(0.6f, 0.6f))) {
        std::cerr << "Failed to load start button texture." << std::endl;
    }

    _cursorPosition = 0;
    _showCursor = true;
    _isSelecting = false;

    if (!_clickBuffer.loadFromFile(pathHelper.getSoundPath("click.ogg"))) {
        std::cerr << "Failed to load click sound effect." << std::endl;
    }
    _clickSound.setBuffer(_clickBuffer);
}

void ConnectionMenu::handleKeyPressed(sf::Event event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
        _settings.toggleShowSettingsOverlay();
    }
    if (!_activeField)
        return;

    std::string activeText = _activeField->getString().toAnsiString();
    if (event.key.code == sf::Keyboard::Left) {
        if (_cursorPosition > 0) {
            _cursorPosition--;
        }
    } else if (event.key.code == sf::Keyboard::Right) {
        if (_cursorPosition < activeText.size()) {
            _cursorPosition++;
        }
    } else if (event.key.code == sf::Keyboard::V &&
               (event.key.control ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem))) {
        std::string clipboardContent =
            sf::Clipboard::getString().toAnsiString();
        activeText.insert(_cursorPosition, clipboardContent);
        _cursorPosition += clipboardContent.length();
        _activeField->setString(activeText);
    }
}

void ConnectionMenu::render(sf::RenderWindow &window) {
    window.draw(_backgroundSprite);
    window.draw(_paramsmenuSprite);

    if (_settings.getShowSettingsOverlay()) {
        window.draw(_overlaySprite);
        window.draw(_TitleSprite);
        _settings.render(window);
    } else {
        window.draw(_nameTitle);
        window.draw(_nameBox);
        window.draw(_nameField);

        window.draw(_ipTitle);
        window.draw(_ipBox);
        window.draw(_ipField);

        window.draw(_portTitle);
        window.draw(_portBox);
        window.draw(_portField);

        window.draw(_backButtonSprite);
        window.draw(_startButtonSprite);

        if (_cursorBlinkClock.getElapsedTime().asSeconds() >= 0.5f) {
            _showCursor = !_showCursor;
            _cursorBlinkClock.restart();
        }

        if (_showCursor && _activeField) {
            std::string activeText = _activeField->getString();
            sf::Vector2f cursorPosition =
                _activeField->findCharacterPos(_cursorPosition);

            sf::RectangleShape cursor(sf::Vector2f(4, 60));
            cursor.setPosition(cursorPosition);
            cursor.setFillColor(sf::Color::White);
            window.draw(cursor);
        }
    }
}

void ConnectionMenu::initInputField(sf::Text &title, sf::Text &textField,
                                    sf::RectangleShape &box,
                                    const std::string &placeholder,
                                    const std::string &labelText,
                                    sf::Vector2f position) {
    title.setFont(_font);
    title.setString(labelText);
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color::Black);
    title.setOutlineThickness(2);
    title.setPosition(position.x, position.y - 10);

    textField.setFont(_font);
    textField.setString(placeholder);
    textField.setCharacterSize(60);
    textField.setFillColor(sf::Color::White);
    textField.setPosition(position.x + 20, position.y + 70);

    box.setSize(sf::Vector2f(800, 100));
    box.setFillColor(sf::Color(128, 128, 128));
    box.setOutlineThickness(0);
    box.setOutlineColor(sf::Color::Blue);
    box.setPosition(position.x, position.y + 60);
}

void ConnectionMenu::handleMouseClick() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(_client.getWindow());
    sf::Vector2f mousePosF = _client.getWindow().mapPixelToCoords(mousePos);
    static bool wasPressed = false;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !wasPressed) {
        if (_paramsmenuSprite.getGlobalBounds().contains(mousePosF)) {
            _settings.toggleShowSettingsOverlay();
            _clickSound.play();
            wasPressed = true;
            return;
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !wasPressed &&
        !_settings.getShowSettingsOverlay()) {
        if (_backButtonSprite.getGlobalBounds().contains(mousePosF)) {
            _client.getSceneManager()->switchScene(SceneManager::LobbyScene);
            _clickSound.play();
            wasPressed = true;
            return;
        }
        if (_startButtonSprite.getGlobalBounds().contains(mousePosF)) {

            std::string ipAddress = _ipField.getString();
            std::string port = _portField.getString();

            _client.playerName = _nameField.getString();
            if (_client.playerName.empty()) {
                _client.playerName = "Player";
            }

            _client.getConnectionProtocol()->connectToServer(ipAddress, port);
            _clickSound.play();

            wasPressed = true;
            return;
        } else if (_nameBox.getGlobalBounds().contains(mousePosF)) {
            _activeField = &_nameField;
            _cursorPosition = _nameField.getString().getSize();
            _nameBox.setOutlineThickness(3);
            _ipBox.setOutlineThickness(0);
            _portBox.setOutlineThickness(0);
        } else if (_ipBox.getGlobalBounds().contains(mousePosF)) {
            _activeField = &_ipField;
            _cursorPosition = _ipField.getString().getSize();
            _ipBox.setOutlineThickness(3);
            _nameBox.setOutlineThickness(0);
            _portBox.setOutlineThickness(0);
        } else if (_portBox.getGlobalBounds().contains(mousePosF)) {
            _activeField = &_portField;
            _cursorPosition = _portField.getString().getSize();
            _portBox.setOutlineThickness(3);
            _nameBox.setOutlineThickness(0);
            _ipBox.setOutlineThickness(0);
        } else {
            _activeField = nullptr;
            _nameBox.setOutlineThickness(0);
            _ipBox.setOutlineThickness(0);
            _portBox.setOutlineThickness(0);
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
    if (_paramsmenuSprite.getGlobalBounds().contains(mousePosF)) {
        _paramsmenuSprite.setColor(sf::Color::Red);
    } else {
        _paramsmenuSprite.setColor(sf::Color::White);
    }
}

void ConnectionMenu::update(float deltaTime) { handleMouseClick(); }

void ConnectionMenu::handleTextInput(sf::Event event) {
    if (_activeField) {
        std::string activeText = _activeField->getString();
        sf::FloatRect textBounds = _activeField->getGlobalBounds();
        float maxWidth = _nameBox.getSize().x - 20;

        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) {
                if (_cursorPosition > 0 && !activeText.empty()) {
                    activeText.erase(_cursorPosition - 1, 1);
                    _cursorPosition--;
                }
            } else if (event.text.unicode < 128) {
                _activeField->setString(activeText +
                                        static_cast<char>(event.text.unicode));
                textBounds = _activeField->getGlobalBounds();

                if (textBounds.width <= maxWidth) {
                    activeText.insert(_cursorPosition, 1,
                                      static_cast<char>(event.text.unicode));
                    _cursorPosition++;
                } else {
                    _activeField->setString(activeText);
                }
            }
        }

        _cursorPosition = std::min(_cursorPosition, activeText.size());
        _activeField->setString(activeText);
        if (textBounds.width > maxWidth) {
            if (&_nameField == _activeField)
                _nameBox.setOutlineColor(sf::Color::Red);
            else if (&_ipField == _activeField)
                _ipBox.setOutlineColor(sf::Color::Red);
            else if (&_portField == _activeField)
                _portBox.setOutlineColor(sf::Color::Red);
        } else {
            _nameBox.setOutlineColor(sf::Color::Blue);
            _ipBox.setOutlineColor(sf::Color::Blue);
            _portBox.setOutlineColor(sf::Color::Blue);
        }
    }
}