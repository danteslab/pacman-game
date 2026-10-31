#include "GridMover.hpp"

#include <cmath>

#include "Globals.hpp"
#include "Maze.hpp"

GridMover::GridMover(float speed)
    : position_{0.0f, 0.0f}, direction_(Direction::None), desiredDirection_(Direction::None),
      tileSize_(static_cast<float>(Globals::TILE_SIZE)), speed_(speed) {}

void GridMover::setPosition(Vector2 position) { position_ = position; }

Vector2 GridMover::position() const { return position_; }

void GridMover::setDirection(Direction direction) { direction_ = direction; }

Direction GridMover::direction() const { return direction_; }

void GridMover::setDesiredDirection(Direction direction) { desiredDirection_ = direction; }

Direction GridMover::desiredDirection() const { return desiredDirection_; }

void GridMover::setSpeed(float speed) { speed_ = speed; }

float GridMover::speed() const { return speed_; }

TileCoord GridMover::tile() const { return tileOf(position_); }

bool GridMover::atTileCenter() const {
  Vector2 center = centerOf(tile());
  return std::fabs(position_.x - center.x) < 0.5f && std::fabs(position_.y - center.y) < 0.5f;
}

bool GridMover::canMove(const Maze &maze, TileCoord current, Direction direction,
                        bool canUseDoor) const {
  Vector2 vector = directionVector(direction);
  TileCoord next = maze.wrap(
      {current.col + static_cast<int>(vector.x), current.row + static_cast<int>(vector.y)});
  return maze.isWalkable(next, canUseDoor);
}

void GridMover::step(const Maze &maze, bool canUseDoor) {
  if (desiredDirection_ != Direction::None && desiredDirection_ == opposite(direction_)) {
    direction_ = desiredDirection_;
  }

  TileCoord current = tile();
  Vector2 center = centerOf(current);
  bool centered = std::fabs(position_.x - center.x) < 0.5f &&
                  std::fabs(position_.y - center.y) < 0.5f;

  if (centered) {
    position_ = center;
    if (desiredDirection_ != Direction::None &&
        canMove(maze, current, desiredDirection_, canUseDoor)) {
      direction_ = desiredDirection_;
    }
    if (direction_ != Direction::None && !canMove(maze, current, direction_, canUseDoor)) {
      direction_ = Direction::None;
    }
  }

  if (direction_ == Direction::None) {
    return;
  }

  Vector2 vector = directionVector(direction_);
  float along = (position_.x - center.x) * vector.x + (position_.y - center.y) * vector.y;

  Vector2 target;
  if (along >= 0.0f) {
    target.x = center.x + vector.x * tileSize_;
    target.y = center.y + vector.y * tileSize_;
  } else {
    target = center;
  }

  float dx = target.x - position_.x;
  float dy = target.y - position_.y;
  float length = std::sqrt(dx * dx + dy * dy);

  if (length <= speed_ || length == 0.0f) {
    position_ = target;
  } else {
    position_.x += dx * (speed_ / length);
    position_.y += dy * (speed_ / length);
  }

  float mazeWidth = static_cast<float>(maze.cols()) * tileSize_;
  if (position_.x < 0.0f) {
    position_.x += mazeWidth;
  } else if (position_.x >= mazeWidth) {
    position_.x -= mazeWidth;
  }
}
