#pragma once
#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "ControllableActor.hpp"
#include "Direction.hpp"
#include "GridMover.hpp"

class Game;
class Maze;

class PacMan : public ControllableActor {
public:
  enum Actions { MOVE_UP = 1, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

  PacMan(Game *game, Maze *maze);
  ~PacMan() override;

  void draw() const override;
  void update() override;
  void doAction(action_t action, int magnitude) override;

  void reset();
  Direction facing() const;
  bool isDead() const;
  void setDead(bool dead);

private:
  Game *game_;
  Maze *maze_;
  GridMover mover_;
  float mouthTimer_;
  bool dead_;
};

#endif // !PACMAN_HPP
