#pragma once
#ifndef GAME_HPP
#define GAME_HPP

#include "ControllableObject.hpp"
#include "Interface.hpp"
#include "Maze.hpp"

class Application;
class ActorManager;
class StageManager;
class ControlManager;
class PacMan;
class GhostManager;
class MazeRenderer;
class Hud;

class Game : public Interface, public ControllableObject {
public:
  explicit Game(Application *app);
  ~Game() override;

  void draw() const override;
  void update() override;
  void doAction(action_t action, int magnitude) override;

  enum class GameActions { PAUSE = 1, RESTART };
  enum class State { Ready, Playing, PacmanDying, LevelClear, GameOver };

  int score() const;
  int lives() const;
  int level() const;
  PacMan *pacman() const;
  Maze &maze();

  void onPelletEaten(PelletResult result);
  void onGhostEaten();
  void onPacmanCaught();

private:
  void resetGame();
  void resetLevel();
  void resetPositions();

  Maze maze_;
  State state_;
  bool paused_;
  float stateTimer_;
  int score_;
  int lives_;
  int level_;

  ActorManager *actorManager_;
  StageManager *stageManager_;
  ControlManager *controlManager_;
  PacMan *pacman_;
  GhostManager *ghostManager_;
  MazeRenderer *mazeRenderer_;
  Hud *hud_;
};

#endif // !GAME_HPP
