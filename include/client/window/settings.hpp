/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** settings
*/

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include "client.hpp"
#include "keymapping.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Settings {
  public:
    Settings(Client &client);
    void initialize();
    void render(sf::RenderWindow &window);
    void handleMouseClick(sf::Event &event, sf::RenderWindow &window);
    void handleButtonHover(sf::RenderWindow &window,
                           sf::RectangleShape &buttonShape);
    float getAspectRatio() const;
    bool getShowSettingsOverlay() const { return _showSettingsOverlay; }
    void toggleShowSettingsOverlay() {
        _showSettingsOverlay = !_showSettingsOverlay;
    }
    void handleKeyPressed(sf::Event &event);
    std::string actionToString(Action action);
    std::string keyToString(sf::Keyboard::Key key);

  private:
    Client &_client;
    sf::Font _font;
    sf::Text _resTitle, _fpsTitle, _soundTitle, _brightnessTitle;

    sf::RectangleShape _button16_9Frame;
    sf::Text _button16_9Text;

    sf::RectangleShape _button4_3Frame;
    sf::Text _button4_3Text;

    sf::RectangleShape _button16_10Frame;
    sf::Text _button16_10Text;

    sf::RectangleShape _button21_9Frame;
    sf::Text _button21_9Text;

    sf::RectangleShape _button30FPSFrame;
    sf::Text _button30FPSText;

    sf::RectangleShape _button60FPSFrame;
    sf::Text _button60FPSText;

    sf::RectangleShape _button144FPSFrame;
    sf::Text _button144FPSText;

    sf::RectangleShape _buttonUnlimitedFPSFrame;
    sf::Text _buttonUnlimitedFPSText;

    sf::RectangleShape _soundPlusFrame;
    sf::Text _soundPlusText;
    sf::RectangleShape _soundMinusFrame;
    sf::Text _soundMinusText;

    sf::RectangleShape _brightnessPlusFrame;
    sf::Text _brightnessPlusText;
    sf::RectangleShape _brightnessMinusFrame;
    sf::Text _brightnessMinusText;

    sf::Text _soundPercentageText;
    sf::Text _brightnessPercentageText;

    std::map<Action, sf::Text> actionTexts;
    std::map<Action, sf::RectangleShape> actionBoxes;
    const Action *activeAction = nullptr;

    float _aspectRatio;
    bool _showSettingsOverlay = false;
};

#endif /* !SETTINGS_HPP_ */
