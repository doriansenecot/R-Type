/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** lasersystem.cpp
*/

#include "lasersystem.hpp"
#include "components.hpp"
#include "coordinator.hpp"
#include <iostream>

extern Coordinator gCoordinator;

void LaserSystem::onEntityDestroyed(Entity entity) {
    auto itAnim = laserAnimations.find(entity);
    if (itAnim != laserAnimations.end()) {
        laserAnimations.erase(itAnim);
    }
    auto itEffect = laserEffects.find(entity);
    if (itEffect != laserEffects.end()) {
        laserEffects.erase(itEffect);
    }
}

void LaserSystem::update(float deltaTime) {
    for (auto &[entity, laser] : laserAnimations) {
        auto &transform = gCoordinator.getComponent<TransformComponent>(entity);
        transform.position +=
            gCoordinator.getComponent<VelocityComponent>(entity).velocity *
            deltaTime;
        laser.currentFrameTime += deltaTime;
        if (laser.currentFrameTime >= laser.frameTime) {
            laser.currentFrame = (laser.currentFrame + 1) % laser.maxFrames;
            laser.textureRect.left =
                laser.currentFrame * laser.textureRect.width;
            laser.sprite.setTextureRect(laser.textureRect);
            laser.currentFrameTime = 0.0f;
        }
        laser.sprite.setPosition(transform.position);
    }

    for (auto it = laserEffects.begin(); it != laserEffects.end();) {
        it->second.elapsedTime += deltaTime;

        it->second.currentFrameTime += deltaTime;
        if (it->second.currentFrameTime >= it->second.frameTime) {
            it->second.currentFrame =
                (it->second.currentFrame + 1) % it->second.maxFrames;

            if (it->second.currentFrame == 0)
                it->second.effectSprite.setTextureRect(
                    sf::IntRect(212, 80, 18, 18));
            else
                it->second.effectSprite.setTextureRect(
                    sf::IntRect(230, 80, 18, 18));

            it->second.currentFrameTime = 0.0f;
        }

        if (it->second.elapsedTime >= it->second.duration) {
            it = laserEffects.erase(it);
        } else {
            auto &spaceshipTransform =
                gCoordinator.getComponent<TransformComponent>(it->first);
            it->second.effectSprite.setPosition(
                spaceshipTransform.position.x + 72,
                spaceshipTransform.position.y - 2);
            ++it;
        }
    }
}

void LaserSystem::render(sf::RenderWindow &window) {
    for (auto &[entity, laser] : laserAnimations) {
        window.draw(laser.sprite);
    }
    for (auto &[entity, effect] : laserEffects) {
        window.draw(effect.effectSprite);
    }
}

void LaserSystem::addLaserEffect(Entity spaceshipEntity) {
    if (!projectileTexture) {
        std::cerr << "Error: Projectile texture is not set!" << std::endl;
        return;
    }
    LaserEffectComponent effect;
    effect.effectSprite.setTexture(*projectileTexture);
    effect.effectSprite.setTextureRect(sf::IntRect(212, 80, 18, 18));
    effect.effectSprite.setScale(2.5f, 2.5f);
    auto &spaceshipTransform =
        gCoordinator.getComponent<TransformComponent>(spaceshipEntity);
    effect.effectSprite.setPosition(spaceshipTransform.position.x + 72,
                                    spaceshipTransform.position.y - 2);
    effect.duration = 0.3f;
    effect.elapsedTime = 0.0f;
    effect.frameTime = 0.1f;
    effect.currentFrameTime = 0.0f;
    effect.currentFrame = 0;
    effect.maxFrames = 2;

    laserEffects[spaceshipEntity] = effect;
}

void LaserSystem::setProjectileTexture(sf::Texture *texture) {
    projectileTexture = texture;
}
