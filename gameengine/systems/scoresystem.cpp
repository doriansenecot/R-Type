/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** scoresystem
*/

#include "scoresystem.hpp"
#include "components.hpp"
#include "coordinator.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

extern Coordinator gCoordinator;

void ScoreSystem::render(sf::RenderWindow &window, sf::Font &font,
                         Entity playerEntity) {
    float startX = 1600.0f;
    float startY = 25.0f;
    float yOffset = startY;
    sf::Text scoreboardTitle;
    scoreboardTitle.setFont(font);
    scoreboardTitle.setCharacterSize(40);
    scoreboardTitle.setFillColor(sf::Color::White);
    scoreboardTitle.setPosition(startX, yOffset);
    scoreboardTitle.setString("Scoreboard :");
    window.draw(scoreboardTitle);
    yOffset += 50;
    for (const auto &[clientId, score] : allScores) {
        std::string playerName = "Player " + std::to_string(clientId);
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        if (clientId == 0) {
            scoreText.setFillColor(sf::Color::Magenta);
        } else if (clientId == 1) {
            scoreText.setFillColor(sf::Color::Cyan);
        } else if (clientId == 2) {
            scoreText.setFillColor(sf::Color::Green);
        } else if (clientId == 3) {
            scoreText.setFillColor(sf::Color::Blue);
        } else if (clientId == 4) {
            scoreText.setFillColor(sf::Color::Red);
        } else {
            scoreText.setFillColor(sf::Color::White);
        }
        scoreText.setPosition(startX, yOffset);
        scoreText.setString(playerName + ": " + std::to_string(score));
        window.draw(scoreText);
        yOffset += 30;
    }
}

void ScoreSystem::updateScore(uint8_t clientId, int newScore) {
    allScores[clientId] = newScore;
}

void ScoreSystem::removeScore(uint8_t clientId) { allScores.erase(clientId); }
