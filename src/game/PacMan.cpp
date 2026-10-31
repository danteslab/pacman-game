#include "PacMan.hpp"

#include <cmath>

#include "Game.hpp"
#include "Globals.hpp"
#include "Maze.hpp"
#include "TileCoord.hpp"

PacMan::PacMan(Game *game, Maze *maze)
    : game_(game), maze_(maze), mover_(static_cast<float>(Globals::TILE_SIZE) / 8.0f),
      mouthTimer_(0.0f), dead_(false) {
  reset();
}

PacMan::~PacMan() {}

void PacMan::reset() {
  mover_.setPosition(centerOf(maze_->pacmanSpawn()));
  mover_.setDirection(Direction::None);
  mover_.setDesiredDirection(Direction::None);
  position = mover_.position();
  mouthTimer_ = 0.0f;
  dead_ = false;
}

void PacMan::update() {
  if (dead_) {
    return;
  }

  mover_.step(*maze_, false);
  position = mover_.position();

  PelletResult result = maze_->consume(mover_.tile());
  if (result != PelletResult::None) {
    game_->onPelletEaten(result);
  }

  mouthTimer_ += GetFrameTime() * 12.0f;
}

void PacMan::doAction(action_t action, int magnitude) {
  (void)magnitude;

  switch (static_cast<Actions>(action)) {
    case Actions::MOVE_UP:
      mover_.setDesiredDirection(Direction::Up);
      break;
    case Actions::MOVE_DOWN:
      mover_.setDesiredDirection(Direction::Down);
      break;
    case Actions::MOVE_LEFT:
      mover_.setDesiredDirection(Direction::Left);
      break;
    case Actions::MOVE_RIGHT:
      mover_.setDesiredDirection(Direction::Right);
      break;
  }
}

void PacMan::draw() const {
  float angle = 0.0f;
  switch (mover_.direction()) {
    case Direction::Up:
      angle = 270.0f;
      break;
    case Direction::Down:
      angle = 90.0f;
      break;
    case Direction::Left:
      angle = 180.0f;
      break;
    default:
      angle = 0.0f;
      break;
  }

  float mouth = 12.0f + 20.0f * (0.5f + 0.5f * std::sin(mouthTimer_));
  float radius = static_cast<float>(Globals::TILE_SIZE) * 0.42f;
  DrawCircleSector(position, radius, angle + mouth * 0.5f, angle + 360.0f - mouth * 0.5f, 32,
                   YELLOW);
}

Direction PacMan::facing() const { return mover_.direction(); }

bool PacMan::isDead() const { return dead_; }

void PacMan::setDead(bool dead) { dead_ = dead; }
