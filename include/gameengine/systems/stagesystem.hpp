/*
** EPITECH PROJECT, 2024
** Tek3
** File description:
** stagesystem
*/

#ifndef STAGESYSTEM_HPP_
#define STAGESYSTEM_HPP_

#include "entity.hpp"
#include "pathhelper.hpp"
#include "protocol.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class Stage {
  public:
    std::string backgroundPath;
    std::vector<EntityType> monsterTypes;
    float spawnInterval;
};

class StageSystem {
  public:
    StageSystem();
    void loadStages();
    void setStage(int stageNumber);
    int getCurrentStage() const { return currentStage; }
    const Stage &getCurrentStageData() const;

  private:
    std::unordered_map<int, Stage> stages;
    int currentStage;
    PathHelper pathHelper;
};

#endif /* !STAGESYSTEM_HPP_ */
