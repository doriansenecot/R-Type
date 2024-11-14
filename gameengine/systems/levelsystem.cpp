/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** levelsystem.cpp
*/

#include "levelsystem.hpp"
#include "coordinator.hpp"
#include <SFML/Graphics.hpp>

extern Coordinator gCoordinator;

void LevelSystem::render(sf::RenderWindow &window, Entity playerEntity,
                         sf::Font &font) {
    if (gCoordinator.hasComponent<XPComponent>(playerEntity) &&
        gCoordinator.hasComponent<LevelComponent>(playerEntity)) {
        auto &xp = gCoordinator.getComponent<XPComponent>(playerEntity);
        auto &level = gCoordinator.getComponent<LevelComponent>(playerEntity);
        sf::RectangleShape xpBarBg(sf::Vector2f(200, 15));
        xpBarBg.setFillColor(sf::Color(50, 50, 50));
        xpBarBg.setPosition(10, 35);
        float xpPercent = static_cast<float>(xp.currentXP) / 100.0f;
        if (xpPercent < 0.0f)
            xpPercent = 0.0f;
        sf::RectangleShape xpBar(sf::Vector2f(200 * xpPercent, 15));
        xpBar.setFillColor(sf::Color::Yellow);
        xpBar.setPosition(10, 35);
        window.draw(xpBarBg);
        window.draw(xpBar);
        sf::Text levelText;
        levelText.setFont(font);
        levelText.setCharacterSize(14);
        levelText.setFillColor(sf::Color::White);
        levelText.setPosition(220, 35);
        levelText.setString("Level: " + std::to_string(level.currentLevel));
        window.draw(levelText);
    }
}
