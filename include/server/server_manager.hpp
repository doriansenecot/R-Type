/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-dorian.senecot
** File description:
** server_manager.hpp
*/

#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include "server.hpp"
#include <map>
#include <memory>

class Server;

class ServerManager {
  public:
    static ServerManager &getInstance();
    void setStartingPort(int port) { startingPort = port; }
    int getStartingPort() const { return startingPort; }
    std::shared_ptr<Server> getServer(int port);
    void addServer(int port, std::shared_ptr<Server> server);
    std::map<int, std::shared_ptr<Server>> getAllServers();

  private:
    ServerManager() = default;
    int startingPort;
    std::map<int, std::shared_ptr<Server>> activeServers;
};

#endif // SERVER_MANAGER_HPP
