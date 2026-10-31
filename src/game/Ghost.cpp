#include "Ghost.hpp"

#include <cstddef>

#include "Game.hpp"
#include "Globals.hpp"
#include "Maze.hpp"
#include "PacMan.hpp"

Ghost::Ghost(GhostKind kind, Game *game, Maze *maze)
    : game_(game), maze_(maze), kind_(kind), state_(GhostState::LeavingHouse),
      mover_(static_cast<float>(Globals::TILE_SIZE) / 10.0f) {
  reset();
}

Ghost::~Ghost() {}

void Ghost::reset() {
  TileCoord spawn = maze_->ghostHouse();
  const std::vector<TileCoord> &spawns = maze_->ghostSpawns();
  size_t index = static_cast<size_t>(kind_);
  if (index < spawns.size()) {
    spawn = spawns[index];
  }

  mover_.setPosition(centerOf(spawn));
  mover_.setDirection(Direction::None);
  mover_.setDesiredDirection(Direction::None);
  position = mover_.position();
  setState(GhostState::LeavingHouse);
}

void Ghost::setState(GhostState state) {
  state_ = state;

  switch (state) {
    case GhostState::Frightened:
      mover_.setSpeed(static_cast<float>(Globals::TILE_SIZE) / 14.0f);
      break;
    case GhostState::Eaten:
      mover_.setSpeed(static_cast<float>(Globals::TILE_SIZE) / 5.0f);
      break;
    default:
      mover_.setSpeed(static_cast<float>(Globals::TILE_SIZE) / 10.0f);
      break;
  }
}

GhostState Ghost::state() const { return state_; }

GhostKind Ghost::kind() const { return kind_; }

TileCoord Ghost::tile() const { return mover_.tile(); }

bool Ghost::isFrightened() const { return state_ == GhostState::Frightened; }

bool Ghost::isEaten() const { return state_ == GhostState::Eaten; }

bool Ghost::isLeavingHouse() const { return state_ == GhostState::LeavingHouse; }

void Ghost::update() {
  if (mover_.atTileCenter()) {
    chooseDirection();
  }

  mover_.step(*maze_, state_ == GhostState::Eaten || state_ == GhostState::LeavingHouse);
  position = mover_.position();
}

void Ghost::chooseDirection() {
  TileCoord current = mover_.tile();

  if (state_ == GhostState::Frightened) {
    const Direction options[4] = {Direction::Up, Direction::Down, Direction::Left,
                                  Direction::Right};
    Direction valid[4];
    int count = 0;

    for (Direction direction : options) {
      if (direction == opposite(mover_.direction())) {
        continue;
      }
      Vector2 vector = directionVector(direction);
      TileCoord next = maze_->wrap({current.col + static_cast<int>(vector.x),
                                    current.row + static_cast<int>(vector.y)});
      if (maze_->isWalkable(next, true)) {
        valid[count++] = direction;
      }
    }

    if (count > 0) {
      mover_.setDirection(valid[GetRandomValue(0, count - 1)]);
    }
    return;
  }

  TileCoord target = targetTile();
  const Direction order[4] = {Direction::Up, Direction::Left, Direction::Down, Direction::Right};
  bool canUseDoor = state_ == GhostState::Eaten || state_ == GhostState::LeavingHouse;
  Direction best = Direction::None;
  float bestDistance = 1.0e9f;

  for (Direction direction : order) {
    if (direction == opposite(mover_.direction())) {
      continue;
    }
    Vector2 vector = directionVector(direction);
    TileCoord next = maze_->wrap({current.col + static_cast<int>(vector.x),
                                  current.row + static_cast<int>(vector.y)});
    if (!maze_->isWalkable(next, canUseDoor)) {
      continue;
    }

    float dx = static_cast<float>(next.col - target.col);
    float dy = static_cast<float>(next.row - target.row);
    float distance = dx * dx + dy * dy;
    if (distance < bestDistance) {
      bestDistance = distance;
      best = direction;
    }
  }

  if (best == Direction::None) {
    best = opposite(mover_.direction());
  }
  mover_.setDirection(best);
}

TileCoord Ghost::targetTile() const {
  switch (state_) {
    case GhostState::LeavingHouse:
      return maze_->ghostDoor();
    case GhostState::Eaten:
      return maze_->ghostHouse();
    case GhostState::Scatter:
      return scatterCorner();
    case GhostState::Chase:
      return chaseTarget();
    default:
      return maze_->ghostHouse();
  }
}

TileCoord Ghost::scatterCorner() const {
  switch (kind_) {
    case GhostKind::Blinky:
      return {maze_->cols() - 1, 0};
    case GhostKind::Pinky:
      return {0, 0};
    case GhostKind::Inky:
      return {maze_->cols() - 1, maze_->rows() - 1};
    case GhostKind::Clyde:
      return {0, maze_->rows() - 1};
  }
  return {0, 0};
}

TileCoord Ghost::chaseTarget() const {
  TileCoord pacmanTile = tileOf(game_->pacman()->position);
  Vector2 facing = directionVector(game_->pacman()->facing());
  int dc = static_cast<int>(facing.x);
  int dr = static_cast<int>(facing.y);

  switch (kind_) {
    case GhostKind::Blinky:
      return pacmanTile;
    case GhostKind::Pinky:
      return {pacmanTile.col + dc * 4, pacmanTile.row + dr * 4};
    case GhostKind::Inky:
      return {pacmanTile.col + dc * 2, pacmanTile.row + dr * 2};
    case GhostKind::Clyde: {
      TileCoord self = mover_.tile();
      int dx = pacmanTile.col - self.col;
      int dy = pacmanTile.row - self.row;
      if (dx * dx + dy * dy > 64) {
        return pacmanTile;
      }
      return scatterCorner();
    }
  }
  return pacmanTile;
}

Color Ghost::bodyColor() const {
  if (state_ == GhostState::Frightened) {
    return BLUE;
  }

  switch (kind_) {
    case GhostKind::Blinky:
      return RED;
    case GhostKind::Pinky:
      return PINK;
    case GhostKind::Inky:
      return SKYBLUE;
    case GhostKind::Clyde:
      return ORANGE;
  }
  return WHITE;
}

void Ghost::draw() const {
  float radius = static_cast<float>(Globals::TILE_SIZE) * 0.42f;
  Color color = bodyColor();

  if (state_ != GhostState::Eaten) {
    DrawCircleV(position, radius, color);
    DrawRectangle(static_cast<int>(position.x - radius), static_cast<int>(position.y),
                  static_cast<int>(radius * 2.0f), static_cast<int>(radius), color);
  }

  float eyeRadius = radius * 0.32f;
  float pupilRadius = eyeRadius * 0.5f;
  Vector2 facing = directionVector(mover_.direction());
  Vector2 leftEye{position.x - radius * 0.35f, position.y - radius * 0.15f};
  Vector2 rightEye{position.x + radius * 0.35f, position.y - radius * 0.15f};

  DrawCircleV(leftEye, eyeRadius, WHITE);
  DrawCircleV(rightEye, eyeRadius, WHITE);
  DrawCircleV({leftEye.x + facing.x * pupilRadius, leftEye.y + facing.y * pupilRadius}, pupilRadius,
              BLACK);
  DrawCircleV({rightEye.x + facing.x * pupilRadius, rightEye.y + facing.y * pupilRadius},
              pupilRadius, BLACK);
}
