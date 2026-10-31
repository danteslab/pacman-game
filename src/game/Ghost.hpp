#pragma once
#ifndef GHOST_HPP
#define GHOST_HPP

#include "Actor.hpp"
#include "Direction.hpp"
#include "GridMover.hpp"
#include "TileCoord.hpp"

class Game;
class Maze;

enum class GhostKind { Blinky, Pinky, Inky, Clyde };
enum class GhostState { LeavingHouse, Scatter, Chase, Frightened, Eaten };

class Ghost : public Actor {
public:
  Ghost(GhostKind kind, Game *game, Maze *maze);
  ~Ghost() override;

  void draw() const override;
  void update() override;

  void reset();
  void setState(GhostState state);

  GhostState state() const;
  GhostKind kind() const;
  TileCoord tile() const;

  bool isFrightened() const;
  bool isEaten() const;
  bool isLeavingHouse() const;

private:
  void chooseDirection();
  TileCoord targetTile() const;
  TileCoord scatterCorner() const;
  TileCoord chaseTarget() const;
  Color bodyColor() const;

  Game *game_;
  Maze *maze_;
  GhostKind kind_;
  GhostState state_;
  GridMover mover_;
};

#endif // !GHOST_HPP
