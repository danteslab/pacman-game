#pragma once
#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <raylib.h>

enum class Direction { None, Up, Down, Left, Right };

inline Vector2 directionVector(Direction direction) {
  switch (direction) {
    case Direction::Up:
      return {0.0f, -1.0f};
    case Direction::Down:
      return {0.0f, 1.0f};
    case Direction::Left:
      return {-1.0f, 0.0f};
    case Direction::Right:
      return {1.0f, 0.0f};
    default:
      return {0.0f, 0.0f};
  }
}

inline Direction opposite(Direction direction) {
  switch (direction) {
    case Direction::Up:
      return Direction::Down;
    case Direction::Down:
      return Direction::Up;
    case Direction::Left:
      return Direction::Right;
    case Direction::Right:
      return Direction::Left;
    default:
      return Direction::None;
  }
}

#endif // !DIRECTION_HPP
