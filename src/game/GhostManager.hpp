#pragma once
#ifndef GHOST_MANAGER_HPP
#define GHOST_MANAGER_HPP

#include <vector>

#include "Actor.hpp"

class Game;
class Maze;
class PacMan;
class Ghost;

class GhostManager : public Actor {
public:
  GhostManager(Game *game, Maze *maze);
  ~GhostManager() override;

  void draw() const override;
  void update() override;

  void reset();
  void frighten();
  void checkCollisions(PacMan *pacman);

private:
  void applyMode();

  Game *game_;
  Maze *maze_;
  std::vector<Ghost *> ghosts_;
  float modeTimer_;
  float frightTimer_;
  bool chase_;
};

#endif // !GHOST_MANAGER_HPP
