/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** windowinit
*/

#include "windowinit.hpp"
#include "utils.hpp"

void WindowInit::initializeWindow(bool fullscreen) {
    sf::ContextSettings settings;

    if (fullscreen) {
        _client._window.create(sf::VideoMode::getDesktopMode(), "R-Type",
                               sf::Style::Fullscreen, settings);
    } else {
        _client._window.create(sf::VideoMode(1920, 1080), "R-Type",
                               sf::Style::Default, settings);
    }

    _client._window.setFramerateLimit(60);
    _client._window.setVerticalSyncEnabled(false);
    _client._view.reset(sf::FloatRect(0, 0, 1920, 1080));
}
