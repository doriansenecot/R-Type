
# Binary Message Format for Network Protocol

This document describes the binary message format used for communication between client and server.

## Message Structure

All messages start with a **MessageHeader** followed by the message type and the message-specific data.

### Message Header (7 bytes)
```cpp
struct MessageHeader {
    uint8_t messageType;
    uint8_t clientId;
    uint32_t magicNumber;
    uint8_t sequenceNumber;
};
```

### Message Types

| Message Type          | Value | Description                                               |
|-----------------------|-------|-----------------------------------------------------------|
| `POSITION_UPDATE`      | 0x01  | Update player or entity position.                         |
| `ENTITY_SPAWN`         | 0x02  | Notify client about a new entity being spawned.            |
| `ENTITY_UPDATE`        | 0x03  | Update an existing entity's state.                        |
| `ENTITY_DESTROY`       | 0x04  | Inform the client to remove an entity.                    |
| `AUTHENTICATION`       | 0x05  | Authenticate the client with the server.                  |
| `ACTION`               | 0x06  | Send player action (e.g., firing a weapon).               |
| `PLAYER_INFO`          | 0x07  | Send player-specific information (e.g., name, score).     |
| `DISCONNECTION`        | 0x08  | Notify about client disconnection.                        |
| `HEALTH_UPDATE`        | 0x09  | Update player or entity health.                           |
| `STAGE_CHANGE`         | 0x0A  | Notify clients about a change in game stage.              |
| `GAME_START`           | 0x0B  | Notify clients that the game has started.                 |
| `EXPERIENCE_UPDATE`    | 0x0C  | Update player experience points.                         |
| `REDIRECT`             | 0x0D  | Redirect client to a different server.                    |
| `CONNECTION_DENIED`    | 0x0E  | Inform client that the connection was denied.             |
| `SCORE_UPDATE`         | 0x0F  | Update player scores.                                    |
| `ACKNOWLEDGMENT`       | 0x10  | Acknowledge receipt of a message.                        |

### Example Messages

#### Position Update Message (25 bytes)
```cpp
struct PositionUpdateMessage {
    MessageHeader header;
    float posX;
    float posY;
    float velocityX;
    float velocityY;
    Direction direction;       
};
```

#### Entity Spawn Message (46 bytes)
```cpp
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
```
