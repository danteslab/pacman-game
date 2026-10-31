#pragma once
#ifndef GRID_MOVER_HPP
#define GRID_MOVER_HPP

#include <raylib.h>

#include "Direction.hpp"
#include "TileCoord.hpp"

class Maze;

class GridMover {
public:
  explicit GridMover(float speed = 0.0f);

  void setPosition(Vector2 position);
  Vector2 position() const;

  void setDirection(Direction direction);
  Direction direction() const;

  void setDesiredDirection(Direction direction);
  Direction desiredDirection() const;

  void setSpeed(float speed);
  float speed() const;

  bool atTileCenter() const;
  TileCoord tile() const;

  void step(const Maze &maze, bool canUseDoor);

private:
  bool canMove(const Maze &maze, TileCoord current, Direction direction, bool canUseDoor) const;

  Vector2 position_;
  Direction direction_;
  Direction desiredDirection_;
  float tileSize_;
  float speed_;
};

#endif // !GRID_MOVER_HPP
