/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** main
*/

#include "server.hpp"

void runServer(std::shared_ptr<Server> server,
               std::unique_ptr<std::atomic<bool>> &serverReady) {
    server->start();
    *serverReady = true;
}

void checkAndSpawnNewServer(int &currentPort,
                            std::vector<std::thread> &serverThreads) {
    auto lastServer = ServerManager::getInstance().getServer(currentPort);
    if (lastServer && (lastServer->getClientCount() >= MAX_CLIENTS ||
                       lastServer->isActive())) {
        while (!lastServer->isUdpPortAvailable(currentPort)) {
            currentPort++;
            if (currentPort > 65535) {
                std::cerr << "No port available." << std::endl;
                return;
            }
        }
        try {
            auto newServer = std::make_shared<Server>(currentPort);
            ServerManager::getInstance().addServer(currentPort, newServer);
            serverThreads.emplace_back([newServer]() { newServer->start(); });
            std::cout << "Launched new server on port : " << currentPort
                      << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error creating new server on port " << currentPort
                      << ": " << e.what() << std::endl;
            currentPort++;
            checkAndSpawnNewServer(currentPort, serverThreads);
        }
    }
}

int main(int argc, char *argv[]) {
    std::map<int, std::shared_ptr<Server>> activeServers;
    int startingPort = 3000;
    if (argc > 1) {
        startingPort = std::stoi(argv[1]);
    }
    std::cout << "Starting server on port " << startingPort << std::endl;
    ServerManager::getInstance().setStartingPort(startingPort);
    std::vector<std::thread> serverThreads;
    std::vector<std::shared_ptr<Server>> servers;
    std::vector<std::unique_ptr<std::atomic<bool>>> serverReadyFlags;

    int currentPort = startingPort;
    auto firstServer = std::make_shared<Server>(currentPort);
    servers.push_back(firstServer);
    serverReadyFlags.emplace_back(std::make_unique<std::atomic<bool>>(false));
    serverThreads.emplace_back([firstServer, &serverReadyFlags, index = 0]() {
        runServer(firstServer, serverReadyFlags[index]);
    });
    while (true) {
        checkAndSpawnNewServer(currentPort, serverThreads);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    for (auto &thread : serverThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    return 0;
}
