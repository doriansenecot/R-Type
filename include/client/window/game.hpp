/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** game
*/

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef GAMESCENE_HPP_
#define GAMESCENE_HPP_

#include "client.hpp"
#include "coordinator.hpp"
#include "explosionsystem.hpp"
#include "healthsystem.hpp"
#include "inputsystem.hpp"
#include "interpolation.hpp"
#include "lasersystem.hpp"
#include "levelsystem.hpp"
#include "movementsystem.hpp"
#include "namerendersystem.hpp"
#include "protocol.hpp"
#include "rendersystem.hpp"
#include "scoresystem.hpp"
#include "settings.hpp"
#include "shadersystem.hpp"
#include "stagesystem.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>

class Game {
  public:
    Game(Client &client);
    void initialize();
    void update(float deltaTime);
    void render(sf::RenderWindow &window);
    void handlePositionUpdate(int clientId, float posX, float posY,
                              float velocityX, float velocityY);
    void handleEntitySpawn(uint32_t entityId, EntityType entityType, float posX,
                           float posY, float velocityX, float velocityY,
                           uint8_t ownerId, int monsterType);
    void handleEntityUpdate(uint32_t entityId, float posX, float posY);
    void handleEntityDestroy(uint32_t entityId);
    void removeClientEntity(int clientId);
    void handlePlayerInfo(int clientId, const std::string &name);
    Entity getEntityByClientId(int clientId) const;
    void setvolumelaser(float volume) { _laserSound.setVolume(volume); }
    void animateMonster(Entity entity, float deltaTime);
    void animatePowerUp(Entity entity, float deltaTime);
    void handleHealthUpdate(int entityId, int currentHealth, int maxHealth);
    void handleStageChange(int newStage);
    void startFadeEffect(bool toBlack);
    void handleExperienceUpdate(int entityId, int currentXP, int currentLevel);
    void toggleWaiting(bool waiting) { isWaiting = waiting; }
    void setupLoadingCircle();
    void updateLoadingCircle(float deltaTime);
    void handleActionMessage(const ActionMessage &msg);
    void handleScoreUpdate(uint8_t clientId, int newScore);
    sf::IntRect getTextureRectForClientID(int clientId);
    void initializePowerUp(Entity newEntity, float posX, float posY);
    void initializeMissile(Entity newEntity, float posX, float posY);
    void initializeMonster(Entity newEntity, float posX, float posY,
                           float velocityX, float velocityY, int monsterType);
    void initializeCommonComponents(Entity newEntity, float posX, float posY,
                                    float width, float height, float velocityX,
                                    float velocityY);
    Entity getPlayerEntity() const { return playerEntity; }
    void setPlayerIsDead(bool isDead);

  private:
    Client &_client;
    std::unordered_map<int, Entity> clientEntities;
    std::unordered_map<uint32_t, Entity> gameEntities;
    ConnectionProtocol _connectionProtocol;
    sf::Texture _backgroundTexture;
    sf::Texture monsterTexture1;
    sf::Texture monsterTexture2;
    sf::Texture monsterTexture3;
    sf::Texture monsterTexture4;
    sf::Texture monsterTexture5;
    sf::Texture monsterTexture6;
    sf::Texture monsterTexture7;
    sf::Texture monsterTexture8;
    sf::Texture monsterTexture9;
    sf::Texture projectileTexturemonster;
    sf::Texture boss1;
    sf::Texture boss2;
    sf::Texture boss3;
    sf::Texture finalboss;
    sf::Texture missileTexture;
    sf::Texture powerUpTexture;
    sf::Texture explosionTexture;
    sf::Sprite _backgroundSprite;
    sf::View _gameView;
    float _scrollSpeed;
    sf::Vector2u _backgroundSize;
    std::shared_ptr<MovementSystem> movementSystem;
    std::shared_ptr<ScoreSystem> scoreSystem;
    std::shared_ptr<RenderSystem> renderSystem;
    std::shared_ptr<InputSystem> inputSystem;
    std::shared_ptr<LaserSystem> laserSystem;
    std::shared_ptr<NameRenderSystem> nameRenderSystem;
    std::shared_ptr<HealthSystem> healthSystem;
    std::shared_ptr<LevelSystem> levelSystem;
    std::shared_ptr<ShaderSystem> shaderSystem;
    std::shared_ptr<ExplosionSystem> explosionSystem;
    float missileCooldownTimer = missileCooldownInterval;

    Entity playerEntity;
    std::unordered_map<int, InterpolationData> interpolationDataMap;
    sf::Texture spaceshipTexture;
    sf::Texture projectileTexture;

    float positionUpdateTimer;
    float positionUpdateInterval;

    sf::Font font;
    sf::Text levelText;

    StageSystem stageSystem;
    sf::Texture currentBackgroundTexture;
    sf::Sprite currentBackgroundSprite;

    std::unordered_map<int, std::string> playerNames;
    sf::SoundBuffer _laserBuffer;
    sf::Sound _laserSound;
    sf::SoundBuffer _explosionBuffer;
    sf::Sound _explosionSound;
    bool playerIsDead = false;

    void setupSpaceship(Entity entity, sf::Vector2f position,
                        sf::IntRect textureRect, const std::string &name = "");
    void setAnimationFrame(Entity entity, sf::Vector2i direction);

    bool isFading = false;
    bool fadeIn = false;
    float fadeAlpha = 0.0f;
    float fadeSpeed = 100.0f;
    sf::RectangleShape fadeOverlay;
    sf::Text stageText;
    bool showStageText = false;
    float stageTextDisplayTime = 0.2f;
    float stageTextTimer = 0.0f;
    bool isTextFading = false;
    float textFadeAlpha = 0.0f;
    float textFadeSpeed = 100.0f;
    sf::RectangleShape waitingArea;
    bool isWaiting = true;
    sf::RectangleShape waitingOverlay;
    sf::Text waitingText;
    sf::VertexArray loadingCircle;
    float loadingRotationSpeed = 100.0f;
    void setupWaitingArea();
    void checkPlayersInWaitingArea();
    float totalTime = 0.0f;
    sf::Clock globalClock;
    std::map<uint32_t, float> entityLastUpdateTime;
    const float entityTimeout = 2.0f;

    sf::Texture _quitButtonTexture;
    sf::Sprite _quitButtonSprite;
    bool _isQuitButtonVisible;

    void handleQuitButtonClick(const sf::Vector2f &mousePos);
    bool _wasEscapePressed = false;
};

#endif /* !GAMESCENE_HPP_ */
