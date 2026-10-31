#include "StageManager.hpp"

#include "ActorManager.hpp"

StageManager::StageManager(ActorManager *actorManager) : actorManager_(actorManager) {}

void StageManager::draw() const { actorManager_->drawAll(); }

StageManager::~StageManager() {}
