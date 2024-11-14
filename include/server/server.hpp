/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** server.hpp
*/

#ifndef SERVER_HPP
#define SERVER_HPP

#include "entity.hpp"
#include "entitymanager.hpp"
#include "movementsystem.hpp"
#include "protocol.hpp"
#include "server_manager.hpp"
#include "system.hpp"
#include <algorithm>
#include <asio.hpp>
#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <unordered_map>
#include <vector>

const int MAX_CLIENTS = 4;
const float MONSTER_SPAWN_INTERVAL = 2000.0f;
const float MONSTER_SPEED_X = -1.0f;
const float MISSILE_SPEED_X = 20.0f;
const float SCREEN_WIDTH = 1920.0f;
const float SCREEN_HEIGHT = 1080.0f;
const int MONSTER_START_DELAY = 3;
const int MAX_RETRIES = 15;
const int ACK_TIMEOUT = 100;

struct ClientInfo {
    asio::ip::udp::endpoint endpoint;
    bool authenticated;
    int id;
    float posX;
    float posY;
    int alive;
    char name[MAX_NAME_LENGTH];
    Entity playerEntity;
    asio::steady_timer timeoutTimer;
    ClientInfo(asio::io_context &io_context)
        : authenticated(false), id(-1), posX(100.0f),
          posY(SCREEN_HEIGHT / 2.0f), playerEntity(-1),
          timeoutTimer(io_context) {
        std::memset(name, 0, MAX_NAME_LENGTH);
    }
};

enum class GameStateWithClients { Waiting, Active };

class Server {
  public:
    Server(int port);
    ~Server();
    void start();
    void stop();
    void sendEntityDestroy(Entity entity);
    void handleStageChange(int newStage);
    bool isActive() const;
    int getClientCount() const;
    std::pair<std::string, uint16_t> findAvailableServer();
    void
    sendConnectionDeniedMessage(const asio::ip::udp::endpoint &clientEndpoint);
    bool isUdpPortAvailable(int port);

  private:
    void receiveFromClients();
    void processMessage(const std::vector<uint8_t> &data,
                        const asio::ip::udp::endpoint &senderEndpoint);
    void removeClient(int clientId);
    void broadcastMessage(const std::vector<uint8_t> &message,
                          int excludeClientId = -1);
    void sendToClient(const std::vector<uint8_t> &message,
                      const asio::ip::udp::endpoint &clientEndpoint);
    void
    handleUnexpectedDisconnect(const asio::ip::udp::endpoint &clientEndpoint);
    void startClientTimeout(std::shared_ptr<ClientInfo> client);
    void sendDisconnectionMessage(int clientId);
    void startGameLoop();
    void updateGame();
    void moveEntities();
    void spawnMonsters();
    void sendEntityUpdates();
    void handleClientAction(int clientId, const ActionMessage &actionMsg);
    void resetGame();
    void authentification(std::shared_ptr<ClientInfo> client,
                          const std::vector<uint8_t> &data);
    void sendEntitySpawn(Entity entity);
    void sendEntitySpawnToClient(Entity entity,
                                 const asio::ip::udp::endpoint &clientEndpoint);
    void handleCollisions();
    void update_life(Entity player, int hp, int offset);
    void spawnPowerUp(float posX, float posY);
    void handleMonsterKilled(Entity monsterEntity, int clientId);
    void sendAllScores();
    bool noActiveMonsters();
    void Spawnstage1(std::chrono::steady_clock::time_point now);
    void Spawnstage2(std::chrono::steady_clock::time_point now);
    void Spawnstage3(std::chrono::steady_clock::time_point now);
    void Spawnstage4(std::chrono::steady_clock::time_point now);
    void SpawnstageOther(std::chrono::steady_clock::time_point now,
                         int bossToSpawn);
    void sendRedirectMessage(const asio::ip::udp::endpoint &clientEndpoint);
    void sendRedirectMessage(const asio::ip::udp::endpoint &clientEndpoint,
                             const std::string &newIp, uint16_t newPort);
    void sendPlayerInfo(std::shared_ptr<ClientInfo> client);
    void sendAuthenticationMessage(std::shared_ptr<ClientInfo> client);
    void sendXPMessages(int ownerId, int currentXP, int currentLevel);
    void sendActionMessage(int clientId);
    void sendHealthMessage(Entity player, HealthComponent &health,
                           EntityManager &entityManager);
    void getClientMessage(std::shared_ptr<ClientInfo> client,
                          const std::vector<uint8_t> &data,
                          const MessageHeader &header);
    void monsterMovement(Entity entity, PositionComponent &pos,
                         VelocityComponent &velocity,
                         EntityManager &entityManager);
    void missileMonster(
        std::vector<std::pair<Entity, Entity>> &missileMonsterCollisions,
        std::unordered_map<Entity, PositionComponent> &positions);
    void playerPowerUp(
        std::vector<std::pair<Entity, Entity>> &playerPowerUpCollisions);
    void lifeModify(Entity player, MonsterType monsterdeg);
    int modifyXp(MonsterType monster);
    void spawnBoss1(float posX, float posY, float velocityX, float velocityY);
    void spawnBoss2(float velocityX, float velocityY);
    void spawnBoss2Mignon(float posX, float posY, float velocityX,
                          float velocityY,
                          std::chrono::steady_clock::time_point now);
    void spawnBoss3(float posX, float posY, float velocityX, float velocityY);
    void spawnBoss3Caracteristic(float posX, float posY, float velocityX,
                                 float velocityY,
                                 std::chrono::steady_clock::time_point now);
    void spawnBoss4(float posX, float posY, float velocityX, float velocityY);
    void spawnBoss4Caracteristic(float posX, float posY, float velocityX,
                                 float velocityY,
                                 std::chrono::steady_clock::time_point now);

    mutable std::mutex clientsMutex;
    EntityManager entityManager;
    std::vector<std::unique_ptr<System>> systems;
    std::chrono::steady_clock::time_point lastUpdateTime;

    asio::io_context io_context;
    asio::ip::udp::socket socket;
    std::unordered_map<int, std::shared_ptr<ClientInfo>> clients;
    std::queue<int> availableIds;
    bool running;
    int port;

    asio::steady_timer gameTimer;
    std::chrono::steady_clock::time_point lastMonsterSpawnTime;
    std::chrono::steady_clock::time_point firstPlayerConnectTime;
    bool firstPlayerConnected = false;
    int monstersKilled = 0;
    int monstersSpawned = 0;
    int monstersToSpawn = 15;
    int difficulty = 1;
    bool bossSpawned = false;
    int currentStage = 1;
    int newBossType = 0;
    void transitionToActive();
    GameStateWithClients currentGameState;

    float waitingAreaPosX = 760.0f;
    float waitingAreaPosY = 340.0f;
    float waitingAreaWidth = 400.0f;
    float waitingAreaHeight = 400.0f;
    std::unordered_map<int, int> clientScores;
    std::atomic<uint32_t> currentSequenceNumber{0};
    struct PendingMessage {
        std::vector<uint8_t> data;
        std::chrono::steady_clock::time_point lastSent;
        int retryCount;
    };

    std::mutex pendingMessagesMutex;
    std::map<uint32_t, PendingMessage> pendingEntityDestroyMessages;
    void startAckTimer(uint32_t sequenceNumber);
    void handleAckTimeout(uint32_t sequenceNumber);
    void handleAcknowledgment(uint32_t sequenceNumber);
    uint32_t generateSequenceNumber();
    std::unordered_map<uint32_t, std::unique_ptr<asio::steady_timer>> ackTimers;
};

#endif // SERVER_HPP
