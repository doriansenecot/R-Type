/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** server.cpp
*/

#include "server.hpp"

Server::Server(int port)
    : port(port),
      socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      running(false), entityManager(),
      currentGameState(GameStateWithClients::Waiting), gameTimer(io_context) {
    if (!socket.is_open()) {
        throw std::runtime_error("Échec de l'ouverture du socket sur le port " +
                                 std::to_string(port));
    }
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        availableIds.push(i);
    }
    lastMonsterSpawnTime = std::chrono::steady_clock::now();
    systems.push_back(std::make_unique<MovementSystem>());
    lastUpdateTime = std::chrono::steady_clock::now();
}

bool Server::isActive() const {
    std::lock_guard<std::mutex> lock(clientsMutex);
    return currentGameState == GameStateWithClients::Active;
}

int Server::getClientCount() const {
    std::lock_guard<std::mutex> lock(clientsMutex);
    return static_cast<int>(clients.size());
}

Server::~Server() { stop(); }

void Server::stop() {
    running = false;
    io_context.stop();
    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.clear();
    std::cout << "Server stopped." << std::endl;
}

void Server::start() {
    running = true;
    try {
        receiveFromClients();
        startGameLoop();
        std::vector<std::thread> threads;
        unsigned int numThreads = std::thread::hardware_concurrency();
        for (unsigned int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this]() {
                try {
                    io_context.run();
                } catch (const std::exception &e) {
                    std::cerr << "Error in ASIO thread : " << e.what()
                              << std::endl;
                }
            });
        }
        for (auto &thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error when restart server on port " << port << ": "
                  << e.what() << std::endl;
        stop();
    }
}

void Server::receiveFromClients() {
    auto buffer = std::make_shared<std::vector<uint8_t>>(MAX_BUFFER_SIZE);
    auto senderEndpoint = std::make_shared<asio::ip::udp::endpoint>();
    socket.async_receive_from(
        asio::buffer(*buffer), *senderEndpoint,
        [this, buffer, senderEndpoint](std::error_code ec,
                                       std::size_t bytesReceived) {
            if (!ec && bytesReceived > 0) {
                buffer->resize(bytesReceived);
                processMessage(*buffer, *senderEndpoint);
            } else if (ec && ec != asio::error::operation_aborted) {
                handleUnexpectedDisconnect(*senderEndpoint);
            }
            if (running) {
                receiveFromClients();
            }
        });
}

void Server::getClientMessage(std::shared_ptr<ClientInfo> client,
                              const std::vector<uint8_t> &data,
                              const MessageHeader &header) {
    startClientTimeout(client);
    switch (static_cast<MessageType>(header.messageType)) {
    case MessageType::AUTHENTICATION: {
        authentification(client, data);
        break;
    }
    case MessageType::ACKNOWLEDGMENT: {
        if (data.size() >= sizeof(AcknowledgmentMessage)) {
            AcknowledgmentMessage ackMsg;
            std::memcpy(&ackMsg, data.data(), sizeof(AcknowledgmentMessage));
            handleAcknowledgment(ackMsg.acknowledgedSequenceNumber);
        }
        break;
    }
    case MessageType::POSITION_UPDATE: {
        if (client->authenticated && client->alive) {
            if (data.size() == sizeof(PositionUpdateMessage)) {
                PositionUpdateMessage posMsg;
                std::memcpy(&posMsg, data.data(),
                            sizeof(PositionUpdateMessage));
                posMsg.header.magicNumber = MAGIC_NUMBER;
                client->posX = posMsg.posX;
                client->posY = posMsg.posY;

                if (entityManager.hasComponent<PositionComponent>(
                        client->playerEntity)) {
                    PositionComponent &pos =
                        entityManager.getComponent<PositionComponent>(
                            client->playerEntity);
                    pos.x = client->posX;
                    pos.y = client->posY;
                }

                broadcastMessage(data, client->id);
            }
        }
        break;
    }
    case MessageType::ACTION: {
        if (client->authenticated && client->alive) {
            if (data.size() == sizeof(ActionMessage)) {
                ActionMessage actionMsg;
                std::memcpy(&actionMsg, data.data(), sizeof(ActionMessage));
                handleClientAction(client->id, actionMsg);
            }
        }
        break;
    }
    case MessageType::DISCONNECTION: {
        removeClient(client->id);
        break;
    }
    default: {
        std::cerr << "Unknown message type received from client " << client->id
                  << "." << std::endl;
        break;
    }
    }
}

void Server::processMessage(const std::vector<uint8_t> &data,
                            const asio::ip::udp::endpoint &senderEndpoint) {
    if (data.size() < sizeof(MessageHeader))
        return;
    MessageHeader header;
    std::memcpy(&header, data.data(), sizeof(MessageHeader));
    if (header.magicNumber != MAGIC_NUMBER) {
        std::cerr << "Wrong id in server side, command ignored." << std::endl;
        return;
    }
    std::shared_ptr<ClientInfo> client;
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        if (header.messageType ==
            static_cast<uint8_t>(MessageType::AUTHENTICATION)) {
            AuthenticationMessage authMsg;
            std::memcpy(&authMsg, data.data(), sizeof(AuthenticationMessage));
            int startingPort = ServerManager::getInstance().getStartingPort();
            if (authMsg.port != startingPort) {
                std::cerr << "Connection attempt denied on port "
                          << authMsg.port << ", allowed only on starting port "
                          << startingPort << "." << std::endl;
                sendConnectionDeniedMessage(senderEndpoint);
                return;
            }
        }
        auto it =
            std::find_if(clients.begin(), clients.end(), [&](const auto &pair) {
                return pair.second->endpoint == senderEndpoint;
            });
        if (it == clients.end() &&
            (clients.size() >= MAX_CLIENTS ||
             currentGameState == GameStateWithClients::Active)) {
            std::cout << "Server full or active. Redirecting new client."
                      << std::endl;
            sendRedirectMessage(senderEndpoint);
            return;
        }
        if (it != clients.end()) {
            client = it->second;
        } else {
            if (availableIds.empty()) {
                std::cerr << "Maximum number of clients reached. Rejecting new "
                             "client."
                          << std::endl;
                return;
            }
            client = std::make_shared<ClientInfo>(io_context);
            client->endpoint = senderEndpoint;
            client->authenticated = false;
            client->alive = 1;
            client->id = availableIds.front();
            availableIds.pop();
            clients[client->id] = client;
            std::cout << "New client assigned ID " << client->id << "."
                      << std::endl;
        }
    }
    getClientMessage(client, data, header);
}

void Server::authentification(std::shared_ptr<ClientInfo> client,
                              const std::vector<uint8_t> &data) {
    client->authenticated = true;
    AuthenticationMessage authMsg;
    if (data.size() >= sizeof(AuthenticationMessage)) {
        std::memcpy(&authMsg, data.data(), sizeof(AuthenticationMessage));
        std::strncpy(client->name, authMsg.name, MAX_NAME_LENGTH);
    } else {
        std::strcpy(client->name, "Player");
    }
    std::cout << "Client " << client->id << " authenticated with name "
              << client->name << "." << std::endl;
    if (!firstPlayerConnected) {
        firstPlayerConnected = true;
        firstPlayerConnectTime = std::chrono::steady_clock::now();
    }

    sendAuthenticationMessage(client);
    client->posX = 100.0f;
    client->posY = SCREEN_HEIGHT / 2.0f;
    Entity playerEntity = entityManager.create_entity();
    client->playerEntity = playerEntity;
    entityManager.addComponent<TypeComponent>(
        playerEntity, TypeComponent{EntityType::Player});
    entityManager.addComponent<PositionComponent>(
        playerEntity, PositionComponent{client->posX, client->posY});
    entityManager.addComponent<VelocityComponent>(
        playerEntity, VelocityComponent{sf::Vector2<float>(0, 0)});
    entityManager.addComponent<OwnerComponent>(playerEntity,
                                               OwnerComponent{client->id});
    entityManager.addComponent<HealthComponent>(playerEntity,
                                                HealthComponent{100, 100});
    entityManager.addComponent<XPComponent>(playerEntity, XPComponent{0});
    entityManager.addComponent<LevelComponent>(playerEntity, LevelComponent{1});
    auto &types = entityManager.getComponentMap<TypeComponent>();
    for (const auto &[entity, typeComp] : types) {
        sendEntitySpawnToClient(entity, client->endpoint);
    }
    sendPlayerInfo(client);
}

void Server::resetGame() {
    lastMonsterSpawnTime = std::chrono::steady_clock::now();
    firstPlayerConnected = false;
    firstPlayerConnectTime = std::chrono::steady_clock::time_point();
    while (!availableIds.empty()) {
        availableIds.pop();
    }
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        availableIds.push(i);
    }
}

void Server::startClientTimeout(std::shared_ptr<ClientInfo> client) {
    client->timeoutTimer.expires_after(std::chrono::seconds(5));
    client->timeoutTimer.async_wait([this, client](const std::error_code &ec) {
        if (!ec) {
            sendDisconnectionMessage(client->id);
            handleUnexpectedDisconnect(client->endpoint);
        }
    });
}

void Server::handleUnexpectedDisconnect(
    const asio::ip::udp::endpoint &senderEndpoint) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->second->endpoint == senderEndpoint) {
            int clientId = it->first;
            std::cerr << "Client " << clientId << " unexpectedly disconnected."
                      << std::endl;
            it->second->timeoutTimer.cancel();
            if (it->second->playerEntity != -1) {
                entityManager.destroy_entity(it->second->playerEntity);
            }
            clients.erase(it);
            availableIds.push(clientId);
            break;
        }
    }
    if (clients.empty()) {
        resetGame();
    }
}

void Server::removeClient(int clientId) {
    sendDisconnectionMessage(clientId);
    std::lock_guard<std::mutex> lock(clientsMutex);
    auto it = clients.find(clientId);
    if (it != clients.end()) {
        std::cout << "Client " << clientId << " disconnected." << std::endl;
        it->second->timeoutTimer.cancel();
        if (it->second->playerEntity != -1) {
            entityManager.destroy_entity(it->second->playerEntity);
        }
        clients.erase(it);
        availableIds.push(clientId);
    } else {
        std::cerr << "Client " << clientId << " not found." << std::endl;
    }
    if (clients.empty()) {
        resetGame();
    }
}

std::string getLocalIpAddress() {
    try {
        asio::io_context io_context;
        asio::ip::udp::resolver resolver(io_context);
        asio::ip::udp::resolver::query query(asio::ip::host_name(), "");
        auto endpoints = resolver.resolve(query);

        for (const auto &endpoint : endpoints) {
            auto ip = endpoint.endpoint().address();
            if (ip.is_v4() && !ip.is_loopback()) {
                return ip.to_string();
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error retrieving local IP address: " << e.what()
                  << std::endl;
    }
    return "127.0.0.1";
}

std::pair<std::string, uint16_t> Server::findAvailableServer() {
    std::string localIp = getLocalIpAddress();
    for (const auto &[port, server] :
         ServerManager::getInstance().getAllServers()) {
        if (server->getClientCount() < MAX_CLIENTS &&
            server->currentGameState == GameStateWithClients::Waiting) {
            return {localIp, static_cast<uint16_t>(port)};
        }
    }
    return {"", 0};
}

bool Server::isUdpPortAvailable(int port) {
    try {
        asio::io_context ioContext;
        asio::ip::udp::socket socket(ioContext);
        asio::error_code ec;
        socket.open(asio::ip::udp::v4(), ec);
        if (ec) {
            return false;
        }
        socket.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), port), ec);
        if (ec) {
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

void Server::sendRedirectMessage(
    const asio::ip::udp::endpoint &clientEndpoint) {
    auto [nextIp, nextPort] = findAvailableServer();
    if (!nextIp.empty() && nextPort != 0) {
        sendRedirectMessage(clientEndpoint, nextIp, nextPort);
        return;
    }
    std::string newIp = getLocalIpAddress();
    int newPort = port + 1;
    while (!isUdpPortAvailable(newPort)) {
        newPort++;
        if (newPort > 65535) {
            std::cerr << "No port available." << std::endl;
            sendConnectionDeniedMessage(clientEndpoint);
            return;
        }
    }
    auto newServer = std::make_shared<Server>(newPort);
    ServerManager::getInstance().addServer(newPort, newServer);
    std::thread newServerThread([newServer]() {
        try {
            newServer->start();
        } catch (const std::exception &e) {
            std::cerr << "Error launching new sub-server : " << e.what()
                      << std::endl;
        }
    });
    newServerThread.detach();
    std::cout << "Launched new server on port : " << newPort << std::endl;
    sendRedirectMessage(clientEndpoint);
}

void Server::startAckTimer(uint32_t sequenceNumber) {
    auto timer = std::make_unique<asio::steady_timer>(
        io_context, std::chrono::milliseconds(ACK_TIMEOUT));
    timer->async_wait([this, sequenceNumber](const std::error_code &ec) {
        if (!ec) {
            handleAckTimeout(sequenceNumber);
        }
    });
    ackTimers[sequenceNumber] = std::move(timer);
}

void Server::handleAckTimeout(uint32_t sequenceNumber) {
    std::lock_guard<std::mutex> lock(pendingMessagesMutex);
    auto it = pendingEntityDestroyMessages.find(sequenceNumber);
    if (it != pendingEntityDestroyMessages.end()) {
        if (it->second.retryCount < MAX_RETRIES) {
            broadcastMessage(it->second.data);
            it->second.retryCount++;
            it->second.lastSent = std::chrono::steady_clock::now();
            startAckTimer(sequenceNumber);
        } else {
            pendingEntityDestroyMessages.erase(it);
            auto timerIt = ackTimers.find(sequenceNumber);
            if (timerIt != ackTimers.end()) {
                timerIt->second->cancel();
                ackTimers.erase(timerIt);
            }
        }
    } else {
        auto timerIt = ackTimers.find(sequenceNumber);
        if (timerIt != ackTimers.end()) {
            timerIt->second->cancel();
            ackTimers.erase(timerIt);
        }
    }
}

void Server::handleAcknowledgment(uint32_t sequenceNumber) {
    std::lock_guard<std::mutex> lock(pendingMessagesMutex);
    auto it = pendingEntityDestroyMessages.find(sequenceNumber);
    if (it != pendingEntityDestroyMessages.end()) {
        pendingEntityDestroyMessages.erase(it);
    }
    auto timerIt = ackTimers.find(sequenceNumber);
    if (timerIt != ackTimers.end()) {
        timerIt->second->cancel();
        ackTimers.erase(timerIt);
    }
}

uint32_t Server::generateSequenceNumber() {
    return currentSequenceNumber.fetch_add(1, std::memory_order_relaxed);
}