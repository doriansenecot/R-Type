/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** healthsystem.cpp
*/

#include "healthsystem.hpp"
#include "coordinator.hpp"
#include <SFML/Graphics.hpp>

extern Coordinator gCoordinator;

void HealthSystem::render(sf::RenderWindow &window, Entity playerEntity) {
    if (gCoordinator.hasComponent<HealthComponent>(playerEntity)) {
        auto &health = gCoordinator.getComponent<HealthComponent>(playerEntity);
        sf::RectangleShape healthBarBg(sf::Vector2f(200, 20));
        healthBarBg.setFillColor(sf::Color::Red);
        healthBarBg.setPosition(10, 10);
        float healthPercent =
            static_cast<float>(health.currentHealth) / health.maxHealth;
        if (static_cast<float>(health.currentHealth) < 0) {
            healthPercent = 0;
        }
        sf::RectangleShape healthBar(sf::Vector2f(200 * healthPercent, 20));
        healthBar.setFillColor(sf::Color::Green);
        healthBar.setPosition(10, 10);
        window.draw(healthBarBg);
        window.draw(healthBar);
    }
    for (auto const &entity : entities) {
        if (gCoordinator.hasComponent<TransformComponent>(entity) &&
            gCoordinator.hasComponent<HealthComponent>(entity) &&
            entity != playerEntity) {
            auto &transform =
                gCoordinator.getComponent<TransformComponent>(entity);
            auto &health = gCoordinator.getComponent<HealthComponent>(entity);

            drawHealthBar(window, transform, health);
        }
    }
}

void HealthSystem::drawHealthBar(sf::RenderWindow &window,
                                 const TransformComponent &transform,
                                 const HealthComponent &health) {
    sf::RectangleShape healthBarBg(sf::Vector2f(50, 5));
    healthBarBg.setFillColor(sf::Color::Red);
    healthBarBg.setPosition(transform.position.x, transform.position.y + 50);

    float healthPercent =
        static_cast<float>(health.currentHealth) / health.maxHealth;
    if (static_cast<float>(health.currentHealth) < 0) {
        healthPercent = 0;
    }
    sf::RectangleShape healthBar(sf::Vector2f(50 * healthPercent, 5));
    healthBar.setFillColor(sf::Color::Green);
    healthBar.setPosition(transform.position.x, transform.position.y + 50);

    window.draw(healthBarBg);
    window.draw(healthBar);
}
