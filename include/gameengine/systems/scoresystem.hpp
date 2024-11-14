/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** scoresystem
*/

#ifndef SCORESYSTEM_HPP_
#define SCORESYSTEM_HPP_

#include "system.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>

class ScoreSystem : public System {
  public:
    void render(sf::RenderWindow &window, sf::Font &font, Entity playerEntity);

    void updateScore(uint8_t clientId, int newScore);
    void removeScore(uint8_t clientId);

  private:
    std::unordered_map<uint8_t, int> allScores;
};

#endif /* !SCORESYSTEM_HPP_ */
