/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** connection_protocol.cpp
*/

#include "connection_protocol.hpp"
#include "client.hpp"
#include "game.hpp"
#include "scenemanager.hpp"

ConnectionProtocol::ConnectionProtocol(Client &client)
    : _client(client),
      socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)),
      strand(io_context.get_executor()) {}

ConnectionProtocol::~ConnectionProtocol() {
    sendDisconnect();
    if (socket.is_open()) {
        asio::error_code ec;
        socket.close(ec);
        if (ec) {
            std::cerr << "Error closing socket: " << ec.message() << std::endl;
        }
    }
    io_context.stop();
    if (networkThread.joinable()) {
        networkThread.join();
    }
}

void ConnectionProtocol::handleEntitySpawn(uint32_t entityId,
                                           EntityType entityType, float posX,
                                           float posY, float velocityX,
                                           float velocityY, uint8_t ownerId,
                                           int monsterType,
                                           int sequenceNumber) {
    {
        _client.ecsCommands.enqueue([this, entityId, entityType, posX, posY,
                                     velocityX, velocityY, ownerId,
                                     monsterType]() {
            _client.getSceneManager()->getGame()->handleEntitySpawn(
                entityId, entityType, posX, posY, velocityX, velocityY, ownerId,
                monsterType);
        });
    }
}

void ConnectionProtocol::handleEntityUpdate(uint32_t entityId, float posX,
                                            float posY) {
    _client.ecsCommands.enqueue([this, entityId, posX, posY]() {
        _client.getSceneManager()->getGame()->handleEntityUpdate(entityId, posX,
                                                                 posY);
    });
}

void ConnectionProtocol::handleEntityDestroy(uint32_t entityId,
                                             uint32_t sequenceNumber) {
    {
        _client.ecsCommands.enqueue([this, entityId]() {
            _client.getSceneManager()->getGame()->handleEntityDestroy(entityId);
        });
    }
}

void ConnectionProtocol::handleDisconnectMessage(MessageHeader header) {
    {
        std::lock_guard<std::mutex> lock(receivedSequencesMutex);
        if (receivedAcknoledgeSequences.find(header.sequenceNumber) !=
            receivedAcknoledgeSequences.end()) {
            sendAcknowledgment(header.sequenceNumber);
            return;
        }
        receivedAcknoledgeSequences.insert(header.sequenceNumber);
        _client.ecsCommands.enqueue([this, header]() {
            _client.getSceneManager()->handleDisconnection(header.clientId);
        });
        sendAcknowledgment(header.sequenceNumber);
    }
}

void ConnectionProtocol::handleStageMessage(StageChangeMessage stageMsg) {
    {
        std::lock_guard<std::mutex> lock(receivedSequencesMutex);
        if (receivedAcknoledgeSequences.find(stageMsg.header.sequenceNumber) !=
            receivedAcknoledgeSequences.end()) {
            sendAcknowledgment(stageMsg.header.sequenceNumber);
            return;
        }
        receivedAcknoledgeSequences.insert(stageMsg.header.sequenceNumber);
        std::cout << "Received stage change message: " << stageMsg.stage
                  << std::endl;
        stageMessageQueue.push(stageMsg.stage);
        sendAcknowledgment(stageMsg.header.sequenceNumber);
    }
}

void ConnectionProtocol::handleActionMessage(ActionMessage actionMsg) {
    {
        _client.ecsCommands.enqueue([this, actionMsg]() {
            _client.getSceneManager()->getGame()->handleActionMessage(
                actionMsg);
        });
    }
}

void ConnectionProtocol::handleHealthMessage(HealthUpdateMessage healthMsg) {
    {
        _client.ecsCommands.enqueue([this, healthMsg]() {
            _client.getSceneManager()->getGame()->handleHealthUpdate(
                healthMsg.header.clientId, healthMsg.currentHealth,
                healthMsg.maxHealth);
        });
    }
}

void ConnectionProtocol::handleInfoMessage(PlayerInfoMessage infoMsg) {
    {
        std::lock_guard<std::mutex> lock(receivedSequencesMutex);
        if (receivedAcknoledgeSequences.find(infoMsg.header.sequenceNumber) !=
            receivedAcknoledgeSequences.end()) {
            sendAcknowledgment(infoMsg.header.sequenceNumber);
            return;
        }
        receivedAcknoledgeSequences.insert(infoMsg.header.sequenceNumber);
        _client.ecsCommands.enqueue([this, infoMsg]() {
            _client.getSceneManager()->getGame()->handlePlayerInfo(
                infoMsg.header.clientId, infoMsg.name);
        });
        sendAcknowledgment(infoMsg.header.sequenceNumber);
    }
}

bool ConnectionProtocol::hasNewStageMessage() {
    return !stageMessageQueue.empty();
}

int ConnectionProtocol::getStageMessage() {
    if (!stageMessageQueue.empty()) {
        int stage = stageMessageQueue.front();
        stageMessageQueue.pop();
        return stage;
    }
    return -1;
}

void ConnectionProtocol::sendDisconnect() {
    MessageHeader disconnectMsg;
    disconnectMsg.messageType =
        static_cast<uint8_t>(MessageType::DISCONNECTION);
    disconnectMsg.clientId = static_cast<uint8_t>(_client.clientId);
    disconnectMsg.magicNumber = MAGIC_NUMBER;
    asio::error_code ec;
    socket.async_send_to(
        asio::buffer(&disconnectMsg, sizeof(disconnectMsg)), serverEndpoint,
        asio::bind_executor(strand, [this](std::error_code ec,
                                           std::size_t /*bytesSent*/) {
            if (ec) {
                std::cerr << "Failed to send disconnect message: "
                          << ec.message() << std::endl;
            } else {
                std::cout << "Sent disconnect message for Client ID: "
                          << static_cast<int>(_client.clientId) << std::endl;
            }
        }));
}

void ConnectionProtocol::connectToServer(const std::string &ipAddress,
                                         const std::string &port) {
    try {
        if (!isConnected) {
            initialPort = port;
        }
        asio::ip::udp::resolver resolver(io_context);
        asio::ip::udp::resolver::results_type endpoints =
            resolver.resolve(asio::ip::udp::v4(), ipAddress, port);
        serverEndpoint = *endpoints.begin();
        std::cout << "Connecting to server at " << ipAddress << ":" << port
                  << std::endl;
        readFromServer();
        if (!networkThread.joinable()) {
            networkThread = std::thread([this]() { io_context.run(); });
        }
        sendAuthentication();
    } catch (std::exception &e) {
        std::cerr << "Failed to resolve server endpoint: " << e.what()
                  << std::endl;
    }
}

void ConnectionProtocol::sendToServer(const std::vector<uint8_t> &messageData) {
    if (serverEndpoint.address().is_unspecified()) {
        std::cerr << "Server endpoint is not specified. Cannot send message."
                  << std::endl;
        return;
    }
    socket.async_send_to(
        asio::buffer(messageData), serverEndpoint,
        asio::bind_executor(
            strand, [](std::error_code ec, std::size_t /*bytesSent*/) {
                if (ec) {
                    std::cerr
                        << "Failed to send message to server: " << ec.message()
                        << std::endl;
                }
            }));
}

void ConnectionProtocol::sendAuthentication() {
    try {
        AuthenticationMessage authMsg;
        authMsg.header.messageType =
            static_cast<uint8_t>(MessageType::AUTHENTICATION);
        authMsg.header.clientId = 0;
        authMsg.header.magicNumber = MAGIC_NUMBER;
        authMsg.port = static_cast<uint16_t>(std::stoi(initialPort));
        std::strncpy(authMsg.name, _client.playerName.c_str(), MAX_NAME_LENGTH);
        authMsg.name[MAX_NAME_LENGTH - 1] = '\0';

        socket.async_send_to(
            asio::buffer(&authMsg, sizeof(authMsg)), serverEndpoint,
            asio::bind_executor(strand, [](std::error_code ec,
                                           std::size_t /*bytesSent*/) {
                if (ec) {
                    std::cerr << "Failed to send authentication message: "
                              << ec.message() << std::endl;
                } else {
                    std::cout << "Authentication message sent." << std::endl;
                }
            }));
    } catch (std::exception &e) {
        std::cerr << "Error sending authentication message: " << e.what()
                  << std::endl;
    }
}

void ConnectionProtocol::sendPositionUpdate(float posX, float posY,
                                            float velocityX, float velocityY,
                                            Direction direction) {
    PositionUpdateMessage posMsg;
    posMsg.header.messageType =
        static_cast<uint8_t>(MessageType::POSITION_UPDATE);
    posMsg.header.clientId = static_cast<uint8_t>(_client.clientId);
    posMsg.header.magicNumber = MAGIC_NUMBER;
    posMsg.posX = posX;
    posMsg.posY = posY;
    posMsg.velocityX = velocityX;
    posMsg.velocityY = velocityY;
    posMsg.direction = direction;
    std::vector<uint8_t> messageData(sizeof(PositionUpdateMessage));
    std::memcpy(messageData.data(), &posMsg, sizeof(PositionUpdateMessage));
    sendToServer(messageData);
}

void ConnectionProtocol::sendAction(uint8_t actionType) {
    ActionMessage actionMsg;
    actionMsg.header.messageType = static_cast<uint8_t>(MessageType::ACTION);
    actionMsg.header.clientId = static_cast<uint8_t>(_client.clientId);
    actionMsg.header.magicNumber = MAGIC_NUMBER;
    actionMsg.actionType = actionType;
    std::vector<uint8_t> messageData(sizeof(ActionMessage));
    std::memcpy(messageData.data(), &actionMsg, sizeof(ActionMessage));
    sendToServer(messageData);
}

void ConnectionProtocol::readFromServer() {
    try {
        auto buffer = std::make_shared<std::vector<uint8_t>>(MAX_BUFFER_SIZE);
        auto senderEndpoint = std::make_shared<asio::ip::udp::endpoint>();
        socket.async_receive_from(
            asio::buffer(*buffer), *senderEndpoint,
            asio::bind_executor(strand, [this, buffer,
                                         senderEndpoint](std::error_code ec,
                                                         std::size_t length) {
                if (!ec && length > 0) {
                    MessageHeader header;
                    std::memcpy(&header, buffer->data(), sizeof(MessageHeader));
                    if (header.magicNumber != MAGIC_NUMBER) {
                        std::cerr << "Wrong id in client side, command ignored."
                                  << std::endl;
                        return;
                    }
                    switch (static_cast<MessageType>(header.messageType)) {
                    case MessageType::AUTHENTICATION: {
                        if (length >= sizeof(AuthenticationResponseMessage)) {
                            AuthenticationResponseMessage authResponse;
                            std::memcpy(&authResponse, buffer->data(),
                                        sizeof(AuthenticationResponseMessage));
                            _client.clientId = authResponse.header.clientId;
                            std::cout << "Assigned client ID: "
                                      << static_cast<int>(_client.clientId)
                                      << std::endl;
                            isConnected = true;

                            _client.ecsCommands.enqueue([this]() {
                                _client.getSceneManager()
                                    ->getGame()
                                    ->initialize();
                                _client.getSceneManager()->switchScene(
                                    SceneManager::GameScene);
                            });
                        }
                        break;
                    }

                    case MessageType::REDIRECT: {
                        if (length >= sizeof(RedirectMessage)) {
                            RedirectMessage redirectMsg;
                            std::memcpy(&redirectMsg, buffer->data(),
                                        sizeof(RedirectMessage));
                            std::string newPort =
                                std::to_string(redirectMsg.newPort);
                            std::cout
                                << "Server is full or active. Redirecting to "
                                << senderEndpoint->address().to_string() << ":"
                                << newPort << std::endl;
                            isConnected = true;
                            connectToServer(
                                senderEndpoint->address().to_string(), newPort);
                        }
                        break;
                    }
                    case MessageType::POSITION_UPDATE: {
                        if (length == sizeof(PositionUpdateMessage)) {
                            PositionUpdateMessage posMsg;
                            std::memcpy(&posMsg, buffer->data(),
                                        sizeof(PositionUpdateMessage));
                            _client.ecsCommands.enqueue([this, posMsg]() {
                                _client.getSceneManager()
                                    ->getGame()
                                    ->handlePositionUpdate(
                                        posMsg.header.clientId, posMsg.posX,
                                        posMsg.posY, posMsg.velocityX,
                                        posMsg.velocityY);
                            });
                        }
                        break;
                    }
                    case MessageType::ENTITY_SPAWN: {
                        if (length == sizeof(EntitySpawnMessage)) {
                            EntitySpawnMessage spawnMsg;
                            std::memcpy(&spawnMsg, buffer->data(),
                                        sizeof(EntitySpawnMessage));
                            handleEntitySpawn(
                                spawnMsg.entityId,
                                static_cast<EntityType>(spawnMsg.entityType),
                                spawnMsg.posX, spawnMsg.posY,
                                spawnMsg.velocityX, spawnMsg.velocityY,
                                spawnMsg.ownerId, spawnMsg.monsterType,
                                header.sequenceNumber);
                        }
                        break;
                    }
                    case MessageType::ENTITY_UPDATE: {
                        if (length == sizeof(EntityUpdateMessage)) {
                            EntityUpdateMessage updateMsg;
                            std::memcpy(&updateMsg, buffer->data(),
                                        sizeof(EntityUpdateMessage));
                            _client.ecsCommands.enqueue([this, updateMsg]() {
                                _client.getSceneManager()
                                    ->getGame()
                                    ->handleEntityUpdate(updateMsg.entityId,
                                                         updateMsg.posX,
                                                         updateMsg.posY);
                            });
                        }
                        break;
                    }
                    case MessageType::ENTITY_DESTROY: {
                        if (length == sizeof(EntityDestroyMessage)) {
                            EntityDestroyMessage destroyMsg;
                            std::memcpy(&destroyMsg, buffer->data(),
                                        sizeof(EntityDestroyMessage));
                            handleEntityDestroy(
                                destroyMsg.entityId,
                                destroyMsg.header.sequenceNumber);
                        }
                        break;
                    }
                    case MessageType::DISCONNECTION: {
                        if (_client.clientId != header.clientId) {
                            std::cout
                            << "Received disconnection message for client "
                            << static_cast<int>(header.clientId) << std::endl;
                        }
                        handleDisconnectMessage(header);
                        break;
                    }
                    case MessageType::PLAYER_INFO: {
                        if (length == sizeof(PlayerInfoMessage)) {
                            PlayerInfoMessage infoMsg;
                            std::memcpy(&infoMsg, buffer->data(),
                                        sizeof(PlayerInfoMessage));
                            handleInfoMessage(infoMsg);
                        }
                        break;
                    }
                    case MessageType::HEALTH_UPDATE: {
                        if (length == sizeof(HealthUpdateMessage)) {
                            HealthUpdateMessage healthMsg;
                            std::memcpy(&healthMsg, buffer->data(),
                                        sizeof(HealthUpdateMessage));
                            handleHealthMessage(healthMsg);
                        }
                        break;
                    }
                    case MessageType::STAGE_CHANGE: {
                        if (length == sizeof(StageChangeMessage)) {
                            StageChangeMessage stageMsg;
                            std::memcpy(&stageMsg, buffer->data(),
                                        sizeof(StageChangeMessage));
                            handleStageMessage(stageMsg);
                        }
                        break;
                    }
                    case MessageType::EXPERIENCE_UPDATE: {
                        if (length == sizeof(ExperienceUpdateMessage)) {
                            ExperienceUpdateMessage expMsg;
                            std::memcpy(&expMsg, buffer->data(),
                                        sizeof(ExperienceUpdateMessage));
                            _client.ecsCommands.enqueue([this, expMsg]() {
                                _client.getSceneManager()
                                    ->getGame()
                                    ->handleExperienceUpdate(
                                        expMsg.header.clientId,
                                        expMsg.currentXP, expMsg.currentLevel);
                            });
                        }
                        break;
                    }
                    case MessageType::GAME_START: {
                        _client.ecsCommands.enqueue([this]() {
                            _client.getSceneManager()->getGame()->toggleWaiting(
                                false);
                        });
                        break;
                    }
                    case MessageType::ACTION: {
                        if (length == sizeof(ActionMessage)) {
                            ActionMessage actionMsg;
                            std::memcpy(&actionMsg, buffer->data(),
                                        sizeof(ActionMessage));
                            handleActionMessage(actionMsg);
                        }
                        break;
                    }
                    case MessageType::SCORE_UPDATE: {
                        if (length == sizeof(ScoreUpdateMessage)) {
                            std::cout << "Received score update message"
                                      << std::endl;
                            ScoreUpdateMessage scoreMsg;
                            std::memcpy(&scoreMsg, buffer->data(),
                                        sizeof(ScoreUpdateMessage));
                            _client.ecsCommands.enqueue([this, scoreMsg]() {
                                _client.getSceneManager()
                                    ->getGame()
                                    ->handleScoreUpdate(
                                        scoreMsg.header.clientId,
                                        scoreMsg.newScore);
                            });
                        }
                        break;
                    }
                    case MessageType::CONNECTION_DENIED: {
                        std::cerr << "Connection denied by server."
                                  << std::endl;
                        _client.ecsCommands.enqueue([this]() {
                            _client.getSceneManager()->switchScene(
                                SceneManager::LobbyScene);
                        });
                        break;
                    }
                    default: {
                        std::cerr
                            << "Unknown message type received from server: "
                            << static_cast<int>(header.messageType)
                            << std::endl;
                        break;
                    }
                    }
                } else if (ec) {
                    std::cerr << "Error receiving from server: " << ec.message()
                              << std::endl;
                }
                readFromServer();
            }));
    } catch (std::exception &e) {
        std::cerr << "Exception in readFromServer: " << e.what() << std::endl;
    }
}

void ConnectionProtocol::sendAcknowledgment(uint32_t sequenceNumber) {
    AcknowledgmentMessage ackMsg;
    ackMsg.header.messageType =
        static_cast<uint8_t>(MessageType::ACKNOWLEDGMENT);
    ackMsg.header.clientId = _client.clientId;
    ackMsg.header.magicNumber = MAGIC_NUMBER;
    ackMsg.header.sequenceNumber = generateClientSequenceNumber();
    ackMsg.acknowledgedSequenceNumber = sequenceNumber;
    std::cout << "Sending acknowledgment for sequence number: "
              << sequenceNumber << std::endl;

    std::vector<uint8_t> ackData(sizeof(AcknowledgmentMessage));
    std::memcpy(ackData.data(), &ackMsg, sizeof(AcknowledgmentMessage));

    sendToServer(ackData);
}

uint32_t ConnectionProtocol::generateClientSequenceNumber() {
    static std::atomic<uint32_t> clientSeqNum{0};
    return clientSeqNum.fetch_add(1, std::memory_order_relaxed);
}