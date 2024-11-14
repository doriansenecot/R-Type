/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** windowinit
*/

#ifndef WINDOWINIT_HPP_
#define WINDOWINIT_HPP_

#include "client.hpp"

class WindowInit {
  public:
    WindowInit(Client &client) : _client(client) {}
    void initializeWindow(bool fullscreen = false);

  private:
    Client &_client;
};

#endif /* !WINDOWINIT_HPP_ */