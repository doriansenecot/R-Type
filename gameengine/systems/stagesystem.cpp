#include "stagesystem.hpp"
#include <iostream>

StageSystem::StageSystem() : currentStage(0) { loadStages(); }

void StageSystem::loadStages() {
    stages[1] = {
        pathHelper.getImagePath("game_red.png"), {EntityType::Monster}, 2.0f};
    stages[2] = {"game_cave.png", {EntityType::Monster}, 1.5f};
    stages[3] = {"game_lava.png", {EntityType::Monster}, 1.5f};
    stages[4] = {"game_final.png", {EntityType::Monster}, 1.5f};
    stages[5] = {"game_red.png", {EntityType::Monster}, 1.5f};
}

void StageSystem::setStage(int stageNumber) {
    if (stages.find(stageNumber) != stages.end()) {
        currentStage = stageNumber;
    }
}

const Stage &StageSystem::getCurrentStageData() const {
    return stages.at(currentStage);
}
