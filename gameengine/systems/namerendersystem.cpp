/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** namerendersystem.cpp
*/

#include "namerendersystem.hpp"
#include "coordinator.hpp"

extern Coordinator gCoordinator;

void NameRenderSystem::render(sf::RenderWindow &window, sf::Font &font) {
    for (auto const &entity : entities) {
        auto &transform = gCoordinator.getComponent<TransformComponent>(entity);
        auto &nameComp = gCoordinator.getComponent<NameComponent>(entity);

        sf::Text nameText;
        nameText.setFont(font);
        nameText.setString(nameComp.name);
        nameText.setCharacterSize(20);
        nameText.setFillColor(sf::Color::White);
        nameText.setOutlineColor(sf::Color::Black);
        nameText.setOutlineThickness(1);

        sf::FloatRect textRect = nameText.getLocalBounds();
        nameText.setOrigin(textRect.left + textRect.width / 2.0f,
                           textRect.top + textRect.height / 2.0f);

        sf::Vector2f position = transform.position;
        position.y -= 20;
        position.x += textRect.width / 2.0f;
        nameText.setPosition(position);

        window.draw(nameText);
    }
}
