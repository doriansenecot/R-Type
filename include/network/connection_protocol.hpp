/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** connection_protocol.hpp
*/

#ifndef CONNECTION_PROTOCOL_HPP_
#define CONNECTION_PROTOCOL_HPP_

#include "protocol.hpp"
#include <asio.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Client;

class ConnectionProtocol {
  public:
    ConnectionProtocol(Client &client);
    ~ConnectionProtocol();

    void connectToServer(const std::string &ipAddress, const std::string &port);
    void sendAuthentication();
    void readFromServer();
    void sendToServer(const std::vector<uint8_t> &messageData);
    void sendDisconnect();
    void sendPositionUpdate(float posX, float posY, float velocityX,
                            float velocityY, Direction direction);
    void sendAction(uint8_t actionType);
    void handleEntitySpawn(uint32_t entityId, EntityType entityType, float posX,
                           float posY, float velocityX, float velocityY,
                           uint8_t ownerId, int monsterType,
                           int sequenceNumber);
    void handleEntityUpdate(uint32_t entityId, float posX, float posY);
    void handleEntityDestroy(uint32_t entityId, uint32_t sequenceNumber);
    void handlePlayerInfo(int clientId, const std::string &name);
    bool hasNewStageMessage();
    int getStageMessage();
    void sendAcknowledgment(uint32_t sequenceNumber);
    uint32_t generateClientSequenceNumber();
    void handleDisconnectMessage(MessageHeader header);
    void handleStageMessage(StageChangeMessage stageMsg);
    void handleActionMessage(ActionMessage actionMsg);
    void handleHealthMessage(HealthUpdateMessage healthMsg);
    void handleInfoMessage(PlayerInfoMessage infoMsg);

  private:
    std::string initialPort;
    bool isConnected = false;
    Client &_client;
    asio::io_context io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint serverEndpoint;
    std::thread networkThread;
    std::mutex socketMutex;
    asio::strand<asio::io_context::executor_type> strand;
    std::queue<int> stageMessageQueue;
    std::mutex receivedSequencesMutex;
    std::unordered_set<uint32_t> receivedAcknoledgeSequences;
};

#endif /* !CONNECTION_PROTOCOL_HPP_ */
