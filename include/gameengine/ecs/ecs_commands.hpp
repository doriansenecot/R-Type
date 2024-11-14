/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** ecs_commands.hpp
*/

#ifndef ECS_COMMANDS_HPP
#define ECS_COMMANDS_HPP

#include <functional>
#include <mutex>
#include <queue>

class ECSCommands {
  public:
    using Command = std::function<void()>;
    void enqueue(Command cmd) {
        std::lock_guard<std::mutex> lock(mtx);
        commands.push(cmd);
    }
    void executeAll() {
        std::lock_guard<std::mutex> lock(mtx);
        while (!commands.empty()) {
            Command cmd = commands.front();
            commands.pop();
            cmd();
        }
    }

  private:
    std::queue<Command> commands;
    std::mutex mtx;
};

#endif // ECS_COMMANDS_HPP
