/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** settings
*/

#include "settings.hpp"
#include "client.hpp"
#include "keymapping.hpp"
#include "lobby.hpp"
#include "pathhelper.hpp"
#include "scenemanager.hpp"
#include "utils.hpp"
#include "windowevent.hpp"
#include <iostream>

Settings::Settings(Client &client)
    : _client(client), _aspectRatio(16.0f / 9.0f) {
    // Constructor
}

void Settings::initialize() {
    PathHelper pathHelper;
    _font.loadFromFile(pathHelper.getFontPath("SHUTTLE-X.ttf"));
    initButtonFrame(_button16_9Frame, sf::Vector2f(150, 50),
                    sf::Vector2f(475, 550), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_button16_9Text, "16:9", sf::Vector2f(520, 555), sf::Color::White,
             30);
    initButtonFrame(_button4_3Frame, sf::Vector2f(150, 50),
                    sf::Vector2f(475, 650), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_button4_3Text, "4:3", sf::Vector2f(525, 655), sf::Color::White,
             30);
    initButtonFrame(_button16_10Frame, sf::Vector2f(150, 50),
                    sf::Vector2f(475, 750), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_button16_10Text, "16:10", sf::Vector2f(515, 755),
             sf::Color::White, 30);
    initButtonFrame(_button21_9Frame, sf::Vector2f(150, 50),
                    sf::Vector2f(475, 850), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_button21_9Text, "21:9", sf::Vector2f(525, 855), sf::Color::White,
             30);
    initText(_resTitle, "Resolution :", sf::Vector2f(400, 450),
             sf::Color::White, 50);

    initText(_fpsTitle, "FPS :", sf::Vector2f(900, 450), sf::Color::White, 50);
    initButtonFrame(_button30FPSFrame, sf::Vector2f(150, 50),
                    sf::Vector2f(875, 550), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_button30FPSText, "30", sf::Vector2f(925, 555), sf::Color::White,
             30);
    initButtonFrame(_button60FPSFrame, sf::Vector2f(150, 50),
                    sf::Vector2f(875, 650), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_button60FPSText, "60", sf::Vector2f(925, 655), sf::Color::White,
             30);
    initButtonFrame(_button144FPSFrame, sf::Vector2f(150, 50),
                    sf::Vector2f(875, 750), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_button144FPSText, "144", sf::Vector2f(920, 755), sf::Color::White,
             30);
    initButtonFrame(_buttonUnlimitedFPSFrame, sf::Vector2f(150, 50),
                    sf::Vector2f(875, 850), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_buttonUnlimitedFPSText, "Unlimited", sf::Vector2f(875, 855),
             sf::Color::White, 30);
    initText(_soundTitle, "Sound :", sf::Vector2f(1275, 450), sf::Color::White,
             50);
    initText(_brightnessTitle, "Brightness :", sf::Vector2f(1200, 685),
             sf::Color::White, 50);
    _resTitle.setFont(_font);
    _fpsTitle.setFont(_font);
    _soundTitle.setFont(_font);
    _brightnessTitle.setFont(_font);
    _button16_9Text.setFont(_font);
    _button4_3Text.setFont(_font);
    _button16_10Text.setFont(_font);
    _button21_9Text.setFont(_font);
    _button30FPSText.setFont(_font);
    _button60FPSText.setFont(_font);
    _button144FPSText.setFont(_font);
    _buttonUnlimitedFPSText.setFont(_font);
    initButtonFrame(_soundPlusFrame, sf::Vector2f(50, 50),
                    sf::Vector2f(1450, 575), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_soundPlusText, "+", sf::Vector2f(1467, 580), sf::Color::White,
             30);
    initButtonFrame(_soundMinusFrame, sf::Vector2f(50, 50),
                    sf::Vector2f(1250, 575), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_soundMinusText, "-", sf::Vector2f(1268, 580), sf::Color::White,
             30);
    _soundMinusText.setFont(_font);
    _soundPlusText.setFont(_font);

    initButtonFrame(_brightnessPlusFrame, sf::Vector2f(50, 50),
                    sf::Vector2f(1450, 815), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_brightnessPlusText, "+", sf::Vector2f(1467, 820),
             sf::Color::White, 30);
    initButtonFrame(_brightnessMinusFrame, sf::Vector2f(50, 50),
                    sf::Vector2f(1250, 815), sf::Color::Transparent,
                    sf::Color::White, 5);
    initText(_brightnessMinusText, "-", sf::Vector2f(1268, 820),
             sf::Color::White, 30);
    _brightnessMinusText.setFont(_font);
    _brightnessPlusText.setFont(_font);

    initText(_soundPercentageText, std::to_string(0) + "%",
             sf::Vector2f(1350, 555), sf::Color::White, 30);
    initText(_brightnessPercentageText, std::to_string(0) + "%",
             sf::Vector2f(1350, 790), sf::Color::White, 30);

    _soundPercentageText.setFont(_font);
    _brightnessPercentageText.setFont(_font);

    std::vector<Action> actions = {Action::MoveUp, Action::MoveDown,
                                   Action::MoveLeft, Action::MoveRight,
                                   Action::Fire};
    float startX = 150;
    float startY = 550;
    float offsetY = 80;

    int index = 0;
    for (Action action : actions) {
        sf::Text actionText;
        sf::RectangleShape actionBox;

        actionText.setFont(_font);
        actionText.setString(
            actionToString(action) + ": " +
            keyToString(_client.getKeyMapping().getKeyForAction(action)));
        actionText.setCharacterSize(30);
        actionText.setFillColor(sf::Color::White);
        actionText.setPosition(startX + 10, startY + index * offsetY + 5);

        actionBox.setSize(sf::Vector2f(250, 40));
        actionBox.setFillColor(sf::Color(0, 0, 0));
        actionBox.setPosition(startX, startY + index * offsetY);
        actionBox.setOutlineThickness(5);
        actionBox.setOutlineColor(sf::Color::White);

        actionTexts[action] = actionText;
        actionBoxes[action] = actionBox;

        index++;
    }
}

void Settings::render(sf::RenderWindow &window) {
    handleButtonHover(window, _button16_9Frame);
    handleButtonHover(window, _button4_3Frame);
    handleButtonHover(window, _button16_10Frame);
    handleButtonHover(window, _button21_9Frame);
    handleButtonHover(window, _button30FPSFrame);
    handleButtonHover(window, _button60FPSFrame);
    handleButtonHover(window, _button144FPSFrame);
    handleButtonHover(window, _buttonUnlimitedFPSFrame);
    handleButtonHover(window, _brightnessMinusFrame);
    handleButtonHover(window, _brightnessPlusFrame);
    handleButtonHover(window, _soundMinusFrame);
    handleButtonHover(window, _soundPlusFrame);
    window.draw(_resTitle);
    window.draw(_fpsTitle);
    window.draw(_soundTitle);
    window.draw(_brightnessTitle);
    window.draw(_button16_9Frame);
    window.draw(_button16_9Text);
    window.draw(_button4_3Frame);
    window.draw(_button4_3Text);
    window.draw(_button16_10Frame);
    window.draw(_button16_10Text);
    window.draw(_button21_9Frame);
    window.draw(_button21_9Text);

    window.draw(_button30FPSFrame);
    window.draw(_button30FPSText);
    window.draw(_button60FPSFrame);
    window.draw(_button60FPSText);
    window.draw(_button144FPSFrame);
    window.draw(_button144FPSText);
    window.draw(_buttonUnlimitedFPSFrame);
    window.draw(_buttonUnlimitedFPSText);

    window.draw(_soundPlusFrame);
    window.draw(_soundPlusText);
    window.draw(_soundMinusFrame);
    window.draw(_soundMinusText);
    window.draw(_soundPercentageText);

    window.draw(_brightnessPlusFrame);
    window.draw(_brightnessPlusText);
    window.draw(_brightnessMinusFrame);
    window.draw(_brightnessMinusText);
    window.draw(_brightnessPercentageText);

    for (const auto &pair : actionBoxes) {
        window.draw(pair.second);
    }
    for (const auto &pair : actionTexts) {
        window.draw(pair.second);
    }

    initText(_soundPercentageText,
             std::to_string(_client.getSceneManager()->getSoundLevel()) + "%",
             sf::Vector2f(1340, 580), sf::Color::White, 30);
    initText(_brightnessPercentageText,
             std::to_string(_client.getSceneManager()->getBrightnessLevel()) +
                 "%",
             sf::Vector2f(1340, 820), sf::Color::White, 30);
}

void Settings::handleMouseClick(sf::Event &event, sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

        if (_button16_9Frame.getGlobalBounds().contains(mousePosF)) {
            _aspectRatio = 16.0f / 9.0f;
            _client.setAspectRatio(_aspectRatio);
            _client.getWindowEvent()->resizeView();
        } else if (_button4_3Frame.getGlobalBounds().contains(mousePosF)) {
            _aspectRatio = 4.0f / 3.0f;
            _client.setAspectRatio(_aspectRatio);
            _client.getWindowEvent()->resizeView();
        } else if (_button16_10Frame.getGlobalBounds().contains(mousePosF)) {
            _aspectRatio = 16.0f / 10.0f;
            _client.setAspectRatio(_aspectRatio);
            _client.getWindowEvent()->resizeView();
        } else if (_button21_9Frame.getGlobalBounds().contains(mousePosF)) {
            _aspectRatio = 21.0f / 9.0f;
            _client.setAspectRatio(_aspectRatio);
            _client.getWindowEvent()->resizeView();
        } else if (_button30FPSFrame.getGlobalBounds().contains(mousePosF)) {
            _client.setFPS(30);
        } else if (_button60FPSFrame.getGlobalBounds().contains(mousePosF)) {
            _client.setFPS(60);
        } else if (_button144FPSFrame.getGlobalBounds().contains(mousePosF)) {
            _client.setFPS(144);
        } else if (_buttonUnlimitedFPSFrame.getGlobalBounds().contains(
                       mousePosF)) {
            _client.setFPS(0);
        }

        if (_soundPlusFrame.getGlobalBounds().contains(mousePosF)) {
            int soundLevel = _client.getSceneManager()->getSoundLevel();
            if (soundLevel < 100) {
                _client.getSceneManager()->setSoundLevel(soundLevel + 10);
            }
        } else if (_soundMinusFrame.getGlobalBounds().contains(mousePosF)) {
            int soundLevel = _client.getSceneManager()->getSoundLevel();
            if (soundLevel > 0) {
                _client.getSceneManager()->setSoundLevel(soundLevel - 10);
            }
        }

        if (_brightnessPlusFrame.getGlobalBounds().contains(mousePosF)) {
            int brightnessLevel =
                _client.getSceneManager()->getBrightnessLevel();
            if (brightnessLevel < 100) {
                _client.getSceneManager()->setBrightnessLevel(brightnessLevel +
                                                              10);
            }
        } else if (_brightnessMinusFrame.getGlobalBounds().contains(
                       mousePosF)) {
            int brightnessLevel =
                _client.getSceneManager()->getBrightnessLevel();
            if (brightnessLevel > 0) {
                _client.getSceneManager()->setBrightnessLevel(brightnessLevel -
                                                              10);
            }
        }

        bool clickedOnAction = false;
        for (auto &pair : actionBoxes) {
            if (pair.second.getGlobalBounds().contains(mousePosF)) {
                activeAction = &pair.first;
                pair.second.setOutlineThickness(5);
                pair.second.setOutlineColor(sf::Color::Red);
                clickedOnAction = true;
            } else {
                pair.second.setOutlineThickness(5);
                pair.second.setOutlineColor(sf::Color::White);
            }
        }
        if (!clickedOnAction) {
            activeAction = nullptr;
        }
    }
}

void Settings::handleButtonHover(sf::RenderWindow &window,
                                 sf::RectangleShape &buttonShape) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF = window.mapPixelToCoords(mousePos);

    if (buttonShape.getGlobalBounds().contains(mousePosF)) {
        buttonShape.setOutlineColor(sf::Color::Red);
    } else {
        buttonShape.setOutlineColor(sf::Color::White);
    }
}

float Settings::getAspectRatio() const { return _aspectRatio; }

std::string Settings::actionToString(Action action) {
    switch (action) {
    case Action::MoveUp:
        return "Up";
    case Action::MoveDown:
        return "Down";
    case Action::MoveLeft:
        return "Left";
    case Action::MoveRight:
        return "Right";
    case Action::Fire:
        return "Fire";
    default:
        return "Unknown Action";
    }
}

std::string Settings::keyToString(sf::Keyboard::Key key) {
    switch (key) {
    case sf::Keyboard::A:
        return "A";
    case sf::Keyboard::B:
        return "B";
    case sf::Keyboard::C:
        return "C";
    case sf::Keyboard::D:
        return "D";
    case sf::Keyboard::E:
        return "E";
    case sf::Keyboard::F:
        return "F";
    case sf::Keyboard::G:
        return "G";
    case sf::Keyboard::H:
        return "H";
    case sf::Keyboard::I:
        return "I";
    case sf::Keyboard::J:
        return "J";
    case sf::Keyboard::K:
        return "K";
    case sf::Keyboard::L:
        return "L";
    case sf::Keyboard::M:
        return "M";
    case sf::Keyboard::N:
        return "N";
    case sf::Keyboard::O:
        return "O";
    case sf::Keyboard::P:
        return "P";
    case sf::Keyboard::Q:
        return "Q";
    case sf::Keyboard::R:
        return "R";
    case sf::Keyboard::S:
        return "S";
    case sf::Keyboard::T:
        return "T";
    case sf::Keyboard::U:
        return "U";
    case sf::Keyboard::V:
        return "V";
    case sf::Keyboard::W:
        return "W";
    case sf::Keyboard::X:
        return "X";
    case sf::Keyboard::Y:
        return "Y";
    case sf::Keyboard::Z:
        return "Z";
    case sf::Keyboard::Space:
        return "Space";
    case sf::Keyboard::Up:
        return "Up";
    case sf::Keyboard::Down:
        return "Down";
    case sf::Keyboard::Left:
        return "Left";
    case sf::Keyboard::Right:
        return "Right";
    default:
        return "Unknown Key";
    }
}

void Settings::handleKeyPressed(sf::Event &event) {
    if (activeAction != nullptr && event.type == sf::Event::KeyPressed) {
        sf::Keyboard::Key newKey = event.key.code;

        _client.getKeyMapping().setKeyForAction(*activeAction, newKey);

        actionTexts[*activeAction].setString(actionToString(*activeAction) +
                                             ": " + keyToString(newKey));

        actionBoxes[*activeAction].setOutlineThickness(5);
        actionBoxes[*activeAction].setOutlineColor(sf::Color::White);
        activeAction = nullptr;
    }
}