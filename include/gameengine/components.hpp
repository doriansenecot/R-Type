/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** components.tpp
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include "protocol.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

struct TransformComponent {
    sf::Vector2f position;
    sf::Vector2f scale;
    float rotation;
};

struct SpriteComponent {
    sf::Sprite sprite;
};

struct VelocityComponent {
    sf::Vector2f velocity;
};

struct MonsterType {
    int type;
};

enum class MovementPattern {
    Zigzag,
    Wave,
    Spiral,
    StraightLine,
    StraightLineSpeed,
    RandomBounce,
    Hover,
    Charge,
    Circular,
    RandomWalk,
    Boss,
    Boss2,
    Boss3,
    BossGround
};

enum class CollisionBehavior { Bounce, Stop, SelfDestruct };

struct CollisionBehaviorComponent {
    CollisionBehavior behavior;
};

struct BehaviorComponent {
    MovementPattern movementPattern;
    CollisionBehavior collisionBehavior;
    bool phaseChanged[3] = {false, false, false};
};

struct PositionComponent {
    float x;
    float y;
};

struct OwnerComponent {
    int ownerId;
};

struct TypeComponent {
    EntityType type;
};

struct InputComponent {
    bool up;
    bool down;
    bool left;
    bool right;
    bool pressed = false;
    bool shoot = false;
    bool spacePressed = false;
    bool spaceReleased = false;
    bool spaceHeld = false;
    bool isFiring;
};

struct BulletComponent {
    float speed;
    sf::IntRect frameRect;
};

struct NameComponent {
    std::string name;
};

struct HealthComponent {
    int currentHealth;
    int maxHealth;
};

struct XPComponent {
    int currentXP;
};

struct LevelComponent {
    int currentLevel;
};

struct DamageComponent {
    int damage;
};

struct ScoreComponent {
    int score = 0;
};

struct SizeComponent {
    float width;
    float height;
};

struct LaserEffectComponent {
    sf::Sprite effectSprite;
    float duration;
    float elapsedTime;
    float frameTime;
    float currentFrameTime;
    int currentFrame;
    int maxFrames;
};

struct ShaderComponent {
    std::shared_ptr<sf::Shader> shader;
    bool useShader = false;
};

struct DeathComponent {
    bool isDying = true;
    float deathTimer = 0.0f;
    int frameIndex = 0;
    float frameTime = 0.1f;
    int numFrames = 5;
};

#endif // COMPONENTS_HPP_
