/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** lobbyevent
*/

#include "client.hpp"
#include "connection_menu.hpp"
#include "lobby.hpp"
#include "scenemanager.hpp"
#include "utils.hpp"
#include "windowevent.hpp"
#include <iostream>

void Lobby::handleMouseHover(sf::RenderWindow &window) {
    handleButtonSpriteHover(window, _playButtonSprite);
    handleButtonSpriteHover(window, _quitButtonSprite);
    handleButtonSpriteHover(window, _paramsSprite);
}

void Lobby::handleButtonHover(sf::RenderWindow &window,
                              sf::RectangleShape &buttonShape) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

    if (buttonShape.getGlobalBounds().contains(mousePosF)) {
        buttonShape.setOutlineColor(sf::Color::Red);
    } else {
        buttonShape.setOutlineColor(sf::Color::White);
    }
}

void Lobby::handleButtonSpriteHover(sf::RenderWindow &window,
                                    sf::Sprite &buttonSprite) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

    if (buttonSprite.getGlobalBounds().contains(mousePosF)) {
        buttonSprite.setColor(sf::Color::Red);
    } else {
        buttonSprite.setColor(sf::Color::White);
    }
}

void Lobby::handleMouseClick(sf::Event &event, sf::RenderWindow &window) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
        _settings.toggleShowSettingsOverlay();
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);
        if (_paramsSprite.getGlobalBounds().contains(mousePosF)) {
            _settings.toggleShowSettingsOverlay();
            _clickSound.play();
        }
        if (_settings.getShowSettingsOverlay()) {
            _settings.handleMouseClick(event, window);
        }
    }
}
