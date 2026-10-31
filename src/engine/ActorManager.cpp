#include "ActorManager.hpp"

#include <algorithm>

#include "Actor.hpp"

ActorManager::ActorManager() {}

ActorManager::~ActorManager() {
  for (auto *actor : actors_) {
    delete actor;
  }
}

void ActorManager::add(Actor *actor) { actors_.push_back(actor); }

void ActorManager::del(Actor *actor) {
  auto it = std::find(actors_.begin(), actors_.end(), actor);
  if (it != actors_.end()) {
    delete *it;
    actors_.erase(it);
  }
}

void ActorManager::update() {
  for (auto *actor : actors_) {
    actor->update();
  }
}

void ActorManager::drawAll() const {
  for (auto *actor : actors_) {
    actor->draw();
  }
}

int ActorManager::size() const { return static_cast<int>(actors_.size()); }
