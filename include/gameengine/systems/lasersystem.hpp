/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** lasersystem
*/

#ifndef LASERSYSTEM_HPP_
#define LASERSYSTEM_HPP_

#include "components.hpp"
#include "coordinator.hpp"
#include "system.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
extern Coordinator gCoordinator;

class LaserSystem : public System {
  public:
    void update(float deltaTime);
    void render(sf::RenderWindow &window);
    void setProjectileTexture(sf::Texture *texture);
    void addLaserEffect(Entity entity);
    void onEntityDestroyed(Entity entity) override;

  private:
    sf::Texture *projectileTexture;

    struct LaserInfo {
        sf::Sprite sprite;
        sf::IntRect textureRect;
        float frameTime;
        float currentFrameTime;
        int currentFrame;
        int maxFrames;
    };

    std::unordered_map<Entity, LaserInfo> laserAnimations;
    std::unordered_map<Entity, LaserEffectComponent> laserEffects;
};

#endif // LASERSYSTEM_HPP
