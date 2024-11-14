/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** windowevent
*/

#ifndef WINDOWEVENT_HPP_
#define WINDOWEVENT_HPP_

#include "client.hpp"
#include "settings.hpp"

class WindowEvent {
  public:
    WindowEvent(Client &client) : _client(client), _settings(client) {}
    void handleWindowEvents();
    void resizeView();
    void toggleFullscreen();

  private:
    Client &_client;
    Settings _settings;
};

#endif /* !WINDOWEVENT_HPP_ */