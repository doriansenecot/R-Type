/*
** EPITECH PROJECT, 2024
** B-CPP-500-LYN-5-1-rtype-dorian.senecot
** File description:
** server_manager.cpp
*/

#include "server_manager.hpp"

ServerManager &ServerManager::getInstance() {
    static ServerManager instance;
    return instance;
}

std::shared_ptr<Server> ServerManager::getServer(int port) {
    auto it = activeServers.find(port);
    if (it != activeServers.end()) {
        return it->second;
    }
    return nullptr;
}

void ServerManager::addServer(int port, std::shared_ptr<Server> server) {
    activeServers[port] = server;
}

std::map<int, std::shared_ptr<Server>> ServerManager::getAllServers() {
    return activeServers;
}
