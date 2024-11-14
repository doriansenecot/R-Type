/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** protocol.hpp
*/

#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <cstddef>
#include <cstdint>

const std::size_t MAX_BUFFER_SIZE = 1024;
const int MAX_NAME_LENGTH = 64;
const float missileCooldownInterval = 0.2f;
const uint32_t MAGIC_NUMBER = 0x85412307;

enum class MessageType : uint8_t {
    AUTHENTICATION = 0x01,
    POSITION_UPDATE = 0x02,
    DISCONNECTION = 0x03,
    ENTITY_SPAWN = 0x04,
    ENTITY_UPDATE = 0x05,
    ENTITY_DESTROY = 0x06,
    ACTION = 0x07,
    PLAYER_INFO = 0x08,
    HEALTH_UPDATE = 0x09,
    STAGE_CHANGE = 0x0A,
    GAME_START = 0x0B,
    EXPERIENCE_UPDATE = 0x0C,
    REDIRECT = 0x0D,
    CONNECTION_DENIED = 0x0E,
    SCORE_UPDATE = 0x0F,
    ACKNOWLEDGMENT = 0x10
};

struct AnimationComponent {
    int frameIndex = 0;
    float timer = 0.0f;
    float frameTime = 0.1f;
    int numFrames;
    int monsterType = -1;
};

enum class Direction : uint8_t { UP, DOWN, LEFT, RIGHT };

enum class EntityType : uint8_t {
    Player = 0x01,
    Monster = 0x02,
    Missile = 0x03,
    PowerUp = 0x04
};

enum class ActionType : uint8_t { Fire = 0x01 };

struct MessageHeader {
    uint8_t messageType;
    uint8_t clientId;
    uint32_t magicNumber;
    uint8_t sequenceNumber;
};

struct AuthenticationMessage {
    MessageHeader header;
    char name[MAX_NAME_LENGTH];
    uint16_t port;
};

struct PlayerInfoMessage {
    MessageHeader header;
    char name[MAX_NAME_LENGTH];
};

struct ScoreUpdateMessage {
    MessageHeader header;
    int newScore;
};

struct AuthenticationResponseMessage {
    MessageHeader header;
};

struct ConnectionDeniedMessage {
    MessageHeader header;
};

struct DisconnectionResponseMessage {
    MessageHeader header;
};

struct PositionUpdateMessage {
    MessageHeader header;
    float posX;
    float posY;
    float velocityX;
    float velocityY;
    Direction direction;
};

struct ActionMessage {
    MessageHeader header;
    uint8_t actionType;
};

struct EntitySpawnMessage {
    MessageHeader header;
    uint32_t entityId;
    uint8_t entityType;
    float posX;
    float posY;
    float velocityX;
    float velocityY;
    int monsterType;
    int currentHealth;
    int damage;
    int maxHealth;
    uint8_t ownerId;
};

struct ExperienceUpdateMessage {
    MessageHeader header;
    int currentXP;
    int currentLevel;
};

struct EntityUpdateMessage {
    MessageHeader header;
    uint32_t entityId;
    float posX;
    float posY;
};

struct EntityDestroyMessage {
    MessageHeader header;
    uint32_t entityId;
};

struct HealthUpdateMessage {
    MessageHeader header;
    int currentHealth;
    int maxHealth;
};

struct StageChangeMessage {
    MessageHeader header;
    uint8_t stage;
};

struct GameStartMessage {
    MessageHeader header;
};

struct RedirectMessage {
    MessageHeader header;
    char newIp[16];
    uint16_t newPort;
};

struct AcknowledgmentMessage {
    MessageHeader header;
    uint32_t acknowledgedSequenceNumber;
};

#endif // PROTOCOL_HPP
