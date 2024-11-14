/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** server_message.cpp
*/

#include "server.hpp"

void Server::transitionToActive() {
    currentGameState = GameStateWithClients::Active;
    std::cout << "Active game state." << std::endl;

    GameStartMessage startMsg;
    startMsg.header.messageType = static_cast<uint8_t>(MessageType::GAME_START);
    startMsg.header.clientId = 0;
    startMsg.header.magicNumber = MAGIC_NUMBER;
    std::vector<uint8_t> messageData(sizeof(GameStartMessage));
    std::memcpy(messageData.data(), &startMsg, sizeof(GameStartMessage));
    broadcastMessage(messageData);

    lastMonsterSpawnTime = std::chrono::steady_clock::now();
}

void Server::sendEntityUpdates() {
    auto &positions = entityManager.getComponentMap<PositionComponent>();

    for (const auto &[entity, position] : positions) {
        TypeComponent &type = entityManager.getComponent<TypeComponent>(entity);
        if (type.type != EntityType::Player) {
            EntityUpdateMessage updateMsg;
            updateMsg.header.messageType =
                static_cast<uint8_t>(MessageType::ENTITY_UPDATE);
            updateMsg.header.clientId = 0;
            updateMsg.header.magicNumber = MAGIC_NUMBER;
            updateMsg.entityId = entity;
            updateMsg.posX = position.x;
            updateMsg.posY = position.y;
            std::vector<uint8_t> messageData(sizeof(EntityUpdateMessage));
            std::memcpy(messageData.data(), &updateMsg,
                        sizeof(EntityUpdateMessage));
            broadcastMessage(messageData);
        }
    }
}

void Server::handleStageChange(int newStage) {
    std::cout << "Switching to stage: " << newStage << std::endl;
    auto &types = entityManager.getComponentMap<TypeComponent>();
    std::vector<Entity> monstersToDestroy;

    for (const auto &[entity, type] : types) {
        if (type.type == EntityType::Monster) {
            monstersToDestroy.push_back(entity);
        }
    }
    for (Entity monster : monstersToDestroy) {
        sendEntityDestroy(monster);
        entityManager.destroy_entity(monster);
    }

    currentStage = newStage;
    StageChangeMessage stageMsg;
    stageMsg.header = {static_cast<uint8_t>(MessageType::STAGE_CHANGE), 0,
                       MAGIC_NUMBER};
    stageMsg.stage = static_cast<uint8_t>(newStage);
    stageMsg.header.sequenceNumber = generateSequenceNumber();
    std::vector<uint8_t> messageData(sizeof(StageChangeMessage));
    std::memcpy(messageData.data(), &stageMsg, sizeof(StageChangeMessage));
    {
        std::lock_guard<std::mutex> lock(pendingMessagesMutex);
        pendingEntityDestroyMessages[stageMsg.header.sequenceNumber] = {
            messageData, std::chrono::steady_clock::now(), 0};
    }
    startAckTimer(stageMsg.header.sequenceNumber);
    broadcastMessage(messageData);
}

void Server::sendEntitySpawn(Entity entity) {
    EntitySpawnMessage spawnMsg;
    spawnMsg.header.messageType =
        static_cast<uint8_t>(MessageType::ENTITY_SPAWN);
    spawnMsg.header.clientId = 0;
    spawnMsg.header.magicNumber = MAGIC_NUMBER;
    spawnMsg.entityId = entity;
    TypeComponent &typeComp = entityManager.getComponent<TypeComponent>(entity);
    spawnMsg.entityType = static_cast<uint8_t>(typeComp.type);
    PositionComponent &posComp =
        entityManager.getComponent<PositionComponent>(entity);
    spawnMsg.posX = posComp.x;
    spawnMsg.posY = posComp.y;
    VelocityComponent &velComp =
        entityManager.getComponent<VelocityComponent>(entity);
    spawnMsg.velocityX = velComp.velocity.x;
    spawnMsg.velocityY = velComp.velocity.y;
    MonsterType &monsterType = entityManager.getComponent<MonsterType>(entity);
    spawnMsg.monsterType = monsterType.type;
    HealthComponent &healthComp =
        entityManager.getComponent<HealthComponent>(entity);
    spawnMsg.currentHealth = healthComp.currentHealth;
    spawnMsg.maxHealth = healthComp.maxHealth;
    OwnerComponent &ownerComp =
        entityManager.getComponent<OwnerComponent>(entity);
    spawnMsg.ownerId = static_cast<uint8_t>(ownerComp.ownerId);
    spawnMsg.header.sequenceNumber = 0;
    std::vector<uint8_t> messageData(sizeof(EntitySpawnMessage));
    std::memcpy(messageData.data(), &spawnMsg, sizeof(EntitySpawnMessage));
    broadcastMessage(messageData);
}

void Server::sendEntitySpawnToClient(
    Entity entity, const asio::ip::udp::endpoint &clientEndpoint) {
    TypeComponent &type = entityManager.getComponent<TypeComponent>(entity);
    if (type.type != EntityType::Player) {
        EntitySpawnMessage spawnMsg;
        spawnMsg.header.messageType =
            static_cast<uint8_t>(MessageType::ENTITY_SPAWN);
        spawnMsg.header.clientId = 0;
        spawnMsg.header.magicNumber = MAGIC_NUMBER;
        spawnMsg.entityId = entity;
        TypeComponent &typeComp =
            entityManager.getComponent<TypeComponent>(entity);
        spawnMsg.entityType = static_cast<uint8_t>(typeComp.type);
        PositionComponent &posComp =
            entityManager.getComponent<PositionComponent>(entity);
        spawnMsg.posX = posComp.x;
        spawnMsg.posY = posComp.y;
        VelocityComponent &velComp =
            entityManager.getComponent<VelocityComponent>(entity);
        spawnMsg.velocityX = velComp.velocity.x;
        spawnMsg.velocityY = velComp.velocity.y;
        OwnerComponent &ownerComp =
            entityManager.getComponent<OwnerComponent>(entity);
        spawnMsg.ownerId = static_cast<uint8_t>(ownerComp.ownerId);
        std::vector<uint8_t> messageData(sizeof(EntitySpawnMessage));
        std::memcpy(messageData.data(), &spawnMsg, sizeof(EntitySpawnMessage));
        sendToClient(messageData, clientEndpoint);
    }
}

void Server::sendEntityDestroy(Entity entity) {
    EntityDestroyMessage destroyMsg;
    destroyMsg.header.messageType =
        static_cast<uint8_t>(MessageType::ENTITY_DESTROY);
    destroyMsg.header.clientId = 0;
    destroyMsg.header.magicNumber = MAGIC_NUMBER;
    destroyMsg.header.sequenceNumber = 0;
    destroyMsg.entityId = entity;

    std::vector<uint8_t> messageData(sizeof(EntityDestroyMessage));
    std::memcpy(messageData.data(), &destroyMsg, sizeof(EntityDestroyMessage));
    broadcastMessage(messageData);
}

void Server::sendDisconnectionMessage(int clientId) {
    DisconnectionResponseMessage deco;
    deco.header.messageType = static_cast<uint8_t>(MessageType::DISCONNECTION);
    deco.header.clientId = clientId;
    deco.header.magicNumber = MAGIC_NUMBER;
    deco.header.sequenceNumber = generateSequenceNumber();
    std::vector<uint8_t> messageData(sizeof(DisconnectionResponseMessage));
    std::memcpy(messageData.data(), &deco,
                sizeof(DisconnectionResponseMessage));
    {
        std::lock_guard<std::mutex> lock(pendingMessagesMutex);
        pendingEntityDestroyMessages[deco.header.sequenceNumber] = {
            messageData, std::chrono::steady_clock::now(), 0};
    }
    startAckTimer(deco.header.sequenceNumber);
    broadcastMessage(messageData);
}

void Server::sendConnectionDeniedMessage(
    const asio::ip::udp::endpoint &clientEndpoint) {
    ConnectionDeniedMessage denyMsg;
    denyMsg.header.messageType =
        static_cast<uint8_t>(MessageType::CONNECTION_DENIED);
    denyMsg.header.clientId = 0;
    denyMsg.header.magicNumber = MAGIC_NUMBER;
    std::vector<uint8_t> messageData(sizeof(ConnectionDeniedMessage));
    std::memcpy(messageData.data(), &denyMsg, sizeof(ConnectionDeniedMessage));
    sendToClient(messageData, clientEndpoint);
}

void Server::sendRedirectMessage(const asio::ip::udp::endpoint &clientEndpoint,
                                 const std::string &newIp, uint16_t newPort) {
    RedirectMessage redirectMsg;
    redirectMsg.header.messageType =
        static_cast<uint8_t>(MessageType::REDIRECT);
    redirectMsg.header.magicNumber = MAGIC_NUMBER;
    std::strncpy(redirectMsg.newIp, newIp.c_str(), sizeof(redirectMsg.newIp));
    redirectMsg.newIp[sizeof(redirectMsg.newIp) - 1] = '\0';
    redirectMsg.newPort = newPort;
    std::vector<uint8_t> messageData(sizeof(RedirectMessage));
    std::memcpy(messageData.data(), &redirectMsg, sizeof(RedirectMessage));
    sendToClient(messageData, clientEndpoint);
}

void Server::sendPlayerInfo(std::shared_ptr<ClientInfo> client) {
    PlayerInfoMessage infoMsg;
    infoMsg.header.messageType = static_cast<uint8_t>(MessageType::PLAYER_INFO);
    infoMsg.header.clientId = client->id;
    infoMsg.header.magicNumber = MAGIC_NUMBER;
    std::strncpy(infoMsg.name, client->name, MAX_NAME_LENGTH);
    infoMsg.name[MAX_NAME_LENGTH - 1] = '\0';
    infoMsg.header.sequenceNumber = generateSequenceNumber();
    std::vector<uint8_t> playerInfoData(sizeof(PlayerInfoMessage));
    std::memcpy(playerInfoData.data(), &infoMsg, sizeof(PlayerInfoMessage));
    {
        std::lock_guard<std::mutex> lock(pendingMessagesMutex);
        pendingEntityDestroyMessages[infoMsg.header.sequenceNumber] = {
            playerInfoData, std::chrono::steady_clock::now(), 0};
    }
    startAckTimer(infoMsg.header.sequenceNumber);
    broadcastMessage(playerInfoData);
    for (const auto &clientPair : clients) {
        if (clientPair.first != client->id) {
            auto &existingClient = clientPair.second;
            PlayerInfoMessage existingInfoMsg;
            existingInfoMsg.header.messageType =
                static_cast<uint8_t>(MessageType::PLAYER_INFO);
            existingInfoMsg.header.clientId = existingClient->id;
            existingInfoMsg.header.magicNumber = MAGIC_NUMBER;
            std::strncpy(existingInfoMsg.name, existingClient->name,
                         MAX_NAME_LENGTH);
            existingInfoMsg.name[MAX_NAME_LENGTH - 1] = '\0';
            existingInfoMsg.header.sequenceNumber = generateSequenceNumber();
            std::vector<uint8_t> existingMessageData(sizeof(PlayerInfoMessage));
            std::memcpy(existingMessageData.data(), &existingInfoMsg,
                        sizeof(PlayerInfoMessage));
            {
                std::lock_guard<std::mutex> lock(pendingMessagesMutex);
                pendingEntityDestroyMessages[existingInfoMsg.header
                                                 .sequenceNumber] = {
                    playerInfoData, std::chrono::steady_clock::now(), 0};
            }
            startAckTimer(existingInfoMsg.header.sequenceNumber);
            sendToClient(existingMessageData, client->endpoint);
        }
    }
}

void Server::sendAuthenticationMessage(std::shared_ptr<ClientInfo> client) {
    AuthenticationResponseMessage responseMsg;
    responseMsg.header.messageType =
        static_cast<uint8_t>(MessageType::AUTHENTICATION);
    responseMsg.header.clientId = client->id;
    responseMsg.header.magicNumber = MAGIC_NUMBER;
    std::vector<uint8_t> messageData(sizeof(AuthenticationResponseMessage));
    std::memcpy(messageData.data(), &responseMsg,
                sizeof(AuthenticationResponseMessage));
    sendToClient(messageData, client->endpoint);
}

void Server::sendXPMessages(int ownerId, int xp, int currentLevel) {
    ExperienceUpdateMessage expMsg;
    expMsg.header.messageType =
        static_cast<uint8_t>(MessageType::EXPERIENCE_UPDATE);
    expMsg.header.clientId = ownerId;
    expMsg.header.magicNumber = MAGIC_NUMBER;
    expMsg.currentXP = xp;
    expMsg.currentLevel = currentLevel;
    std::vector<uint8_t> messageData(sizeof(ExperienceUpdateMessage));
    std::memcpy(messageData.data(), &expMsg, sizeof(ExperienceUpdateMessage));
    broadcastMessage(messageData);
}

void Server::sendActionMessage(int clientId) {
    ActionMessage fireMsg;
    fireMsg.header.messageType = static_cast<uint8_t>(MessageType::ACTION);
    fireMsg.header.clientId = static_cast<uint8_t>(clientId);
    fireMsg.header.magicNumber = MAGIC_NUMBER;
    fireMsg.actionType = static_cast<uint8_t>(ActionType::Fire);
    fireMsg.header.sequenceNumber = 0;
    std::vector<uint8_t> messageData(sizeof(ActionMessage));
    std::memcpy(messageData.data(), &fireMsg, sizeof(ActionMessage));
    broadcastMessage(messageData, -1);
}

void Server::sendHealthMessage(Entity player, HealthComponent &health,
                               EntityManager &entityManager) {
    HealthUpdateMessage healthMsg;
    healthMsg.header.messageType =
        static_cast<uint8_t>(MessageType::HEALTH_UPDATE);
    healthMsg.header.magicNumber = MAGIC_NUMBER;
    if (entityManager.hasComponent<OwnerComponent>(player)) {
        OwnerComponent &owner =
            entityManager.getComponent<OwnerComponent>(player);
        healthMsg.header.clientId = owner.ownerId;
    } else {
        healthMsg.header.clientId = 0;
    }
    healthMsg.currentHealth = health.currentHealth;
    healthMsg.maxHealth = health.maxHealth;
    healthMsg.header.sequenceNumber = 0;
    std::vector<uint8_t> messageData(sizeof(HealthUpdateMessage));
    std::memcpy(messageData.data(), &healthMsg, sizeof(HealthUpdateMessage));
    broadcastMessage(messageData);
}

void Server::sendAllScores() {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (const auto &[clientId, clientInfo] : clients) {
        for (const auto &[otherClientId, score] : clientScores) {
            ScoreUpdateMessage scoreMsg;
            scoreMsg.header.messageType =
                static_cast<uint8_t>(MessageType::SCORE_UPDATE);
            scoreMsg.header.clientId = static_cast<uint8_t>(otherClientId);
            scoreMsg.header.magicNumber = MAGIC_NUMBER;
            scoreMsg.newScore = score;
            std::vector<uint8_t> messageData(sizeof(ScoreUpdateMessage));
            std::memcpy(messageData.data(), &scoreMsg,
                        sizeof(ScoreUpdateMessage));
            sendToClient(messageData, clientInfo->endpoint);
        }
    }
}

void Server::broadcastMessage(const std::vector<uint8_t> &message,
                              int excludeClientId) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto &clientPair : clients) {
        int clientId = clientPair.first;
        auto &client = clientPair.second;
        if (client->authenticated && clientId != excludeClientId) {
            sendToClient(message, client->endpoint);
        }
    }
}

void Server::sendToClient(const std::vector<uint8_t> &message,
                          const asio::ip::udp::endpoint &clientEndpoint) {
    socket.async_send_to(asio::buffer(message), clientEndpoint,
                         [](std::error_code ec, std::size_t /*bytesSent*/) {
                             if (ec) {
                                 std::cerr << "Error sending message: "
                                           << ec.message() << std::endl;
                             }
                         });
}
