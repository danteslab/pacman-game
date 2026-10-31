#include "GhostManager.hpp"

#include <raylib.h>

#include "Game.hpp"
#include "Ghost.hpp"
#include "Globals.hpp"
#include "Maze.hpp"
#include "PacMan.hpp"

GhostManager::GhostManager(Game *game, Maze *maze)
    : game_(game), maze_(maze), modeTimer_(0.0f), frightTimer_(0.0f), chase_(false) {
  ghosts_.push_back(new Ghost(GhostKind::Blinky, game, maze));
  ghosts_.push_back(new Ghost(GhostKind::Pinky, game, maze));
  ghosts_.push_back(new Ghost(GhostKind::Inky, game, maze));
  ghosts_.push_back(new Ghost(GhostKind::Clyde, game, maze));
}

GhostManager::~GhostManager() {
  for (auto *ghost : ghosts_) {
    delete ghost;
  }
}

void GhostManager::draw() const {
  for (auto *ghost : ghosts_) {
    ghost->draw();
  }
}

void GhostManager::update() {
  float delta = GetFrameTime();
  modeTimer_ += delta;

  float wave = chase_ ? 20.0f : 7.0f;
  if (modeTimer_ >= wave) {
    modeTimer_ = 0.0f;
    chase_ = !chase_;
    if (frightTimer_ <= 0.0f) {
      applyMode();
    }
  }

  if (frightTimer_ > 0.0f) {
    frightTimer_ -= delta;
    if (frightTimer_ <= 0.0f) {
      applyMode();
    }
  }

  for (auto *ghost : ghosts_) {
    ghost->update();

    if (ghost->isLeavingHouse() && ghost->tile().row <= maze_->ghostDoor().row - 1) {
      ghost->setState(chase_ ? GhostState::Chase : GhostState::Scatter);
    }

    if (ghost->isEaten() && ghost->tile() == maze_->ghostHouse()) {
      ghost->setState(chase_ ? GhostState::Chase : GhostState::Scatter);
    }
  }
}

void GhostManager::reset() {
  modeTimer_ = 0.0f;
  frightTimer_ = 0.0f;
  chase_ = false;
  for (auto *ghost : ghosts_) {
    ghost->reset();
  }
}

void GhostManager::frighten() {
  frightTimer_ = 7.0f;
  for (auto *ghost : ghosts_) {
    if (!ghost->isEaten()) {
      ghost->setState(GhostState::Frightened);
    }
  }
}

void GhostManager::applyMode() {
  for (auto *ghost : ghosts_) {
    if (!ghost->isLeavingHouse() && !ghost->isEaten()) {
      ghost->setState(chase_ ? GhostState::Chase : GhostState::Scatter);
    }
  }
}

void GhostManager::checkCollisions(PacMan *pacman) {
  float radius = static_cast<float>(Globals::TILE_SIZE) * 0.6f;

  for (auto *ghost : ghosts_) {
    if (ghost->isEaten()) {
      continue;
    }

    float dx = ghost->position.x - pacman->position.x;
    float dy = ghost->position.y - pacman->position.y;

    if (dx * dx + dy * dy <= radius * radius) {
      if (ghost->isFrightened()) {
        ghost->setState(GhostState::Eaten);
        game_->onGhostEaten();
      } else {
        game_->onPacmanCaught();
      }
      return;
    }
  }
}
