#include "Game.hpp"

#include <raylib.h>

#include "ActorManager.hpp"
#include "Application.hpp"
#include "Control.hpp"
#include "ControlManager.hpp"
#include "GhostManager.hpp"
#include "Globals.hpp"
#include "Hud.hpp"
#include "Keyboard.hpp"
#include "MazeRenderer.hpp"
#include "PacMan.hpp"
#include "Peripheral.hpp"
#include "StageManager.hpp"

Game::Game(Application *app)
    : Interface(app), state_(State::Ready), paused_(false), stateTimer_(0.0f), score_(0),
      lives_(3), level_(1), actorManager_(nullptr), stageManager_(nullptr),
      controlManager_(nullptr), pacman_(nullptr), ghostManager_(nullptr), mazeRenderer_(nullptr),
      hud_(nullptr) {
  maze_.loadDefault();

  actorManager_ = new ActorManager();
  stageManager_ = new StageManager(actorManager_);
  controlManager_ = new ControlManager();

  mazeRenderer_ = new MazeRenderer(&maze_);
  pacman_ = new PacMan(this, &maze_);
  ghostManager_ = new GhostManager(this, &maze_);
  hud_ = new Hud(this);

  actorManager_->add(mazeRenderer_);
  actorManager_->add(ghostManager_);
  actorManager_->add(pacman_);
  actorManager_->add(hud_);

  Keyboard *keyboard = app->getKeyboard();
  controlManager_->addPeripheral(keyboard);

  Control *pacmanControl = new Control();
  pacmanControl->setOwner(pacman_);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_UP, keyboard, KEY_UP,
                                     Peripheral::ON_PRESSING);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_UP, keyboard, KEY_W,
                                     Peripheral::ON_PRESSING);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_DOWN, keyboard, KEY_DOWN,
                                     Peripheral::ON_PRESSING);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_DOWN, keyboard, KEY_S,
                                     Peripheral::ON_PRESSING);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_LEFT, keyboard, KEY_LEFT,
                                     Peripheral::ON_PRESSING);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_LEFT, keyboard, KEY_A,
                                     Peripheral::ON_PRESSING);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_RIGHT, keyboard, KEY_RIGHT,
                                     Peripheral::ON_PRESSING);
  pacmanControl->addActionPeripheral(PacMan::Actions::MOVE_RIGHT, keyboard, KEY_D,
                                     Peripheral::ON_PRESSING);
  controlManager_->addControl(pacmanControl);

  Control *gameControl = new Control();
  gameControl->setOwner(this);
  gameControl->addActionPeripheral(static_cast<ControllableObject::action_t>(GameActions::PAUSE),
                                   keyboard, KEY_P, Peripheral::ON_PRESS);
  gameControl->addActionPeripheral(static_cast<ControllableObject::action_t>(GameActions::RESTART),
                                   keyboard, KEY_ENTER, Peripheral::ON_PRESS);
  controlManager_->addControl(gameControl);

  resetGame();
}

Game::~Game() {
  delete controlManager_;
  delete actorManager_;
  delete stageManager_;
}

int Game::score() const { return score_; }

int Game::lives() const { return lives_; }

int Game::level() const { return level_; }

PacMan *Game::pacman() const { return pacman_; }

Maze &Game::maze() { return maze_; }

void Game::draw() const {
  DrawRectangle(0, 0, Globals::Settings::WIDTH, Globals::Settings::HEIGHT, BLACK);
  stageManager_->draw();

  const char *message = nullptr;
  switch (state_) {
    case State::Ready:
      message = "READY!";
      break;
    case State::PacmanDying:
      message = "OUCH!";
      break;
    case State::LevelClear:
      message = "LEVEL CLEAR";
      break;
    case State::GameOver:
      message = "GAME OVER - ENTER TO RESTART";
      break;
    default:
      break;
  }

  if (message != nullptr) {
    int fontSize = 22;
    int width = MeasureText(message, fontSize);
    int y = Globals::MAZE_OFFSET_Y + (Globals::MAZE_ROWS * Globals::TILE_SIZE) / 2;
    DrawText(message, (Globals::Settings::WIDTH - width) / 2, y, fontSize, YELLOW);
  }

  if (paused_) {
    int fontSize = 22;
    int width = MeasureText("PAUSED", fontSize);
    int y = Globals::MAZE_OFFSET_Y + (Globals::MAZE_ROWS * Globals::TILE_SIZE) / 2 + 32;
    DrawText("PAUSED", (Globals::Settings::WIDTH - width) / 2, y, fontSize, WHITE);
  }
}

void Game::update() {
  controlManager_->update();

  if (paused_) {
    return;
  }

  float delta = GetFrameTime();

  switch (state_) {
    case State::Ready:
      stateTimer_ -= delta;
      if (stateTimer_ <= 0.0f) {
        state_ = State::Playing;
      }
      break;
    case State::Playing:
      actorManager_->update();
      if (state_ == State::Playing) {
        ghostManager_->checkCollisions(pacman_);
      }
      break;
    case State::PacmanDying:
      stateTimer_ -= delta;
      if (stateTimer_ <= 0.0f) {
        --lives_;
        if (lives_ <= 0) {
          state_ = State::GameOver;
        } else {
          resetPositions();
          state_ = State::Ready;
          stateTimer_ = 2.0f;
        }
      }
      break;
    case State::LevelClear:
      stateTimer_ -= delta;
      if (stateTimer_ <= 0.0f) {
        ++level_;
        resetLevel();
      }
      break;
    case State::GameOver:
      break;
  }
}

void Game::doAction(action_t action, int magnitude) {
  (void)magnitude;

  switch (static_cast<GameActions>(action)) {
    case GameActions::PAUSE:
      paused_ = !paused_;
      break;
    case GameActions::RESTART:
      if (state_ == State::GameOver) {
        resetGame();
      }
      break;
  }
}

void Game::onPelletEaten(PelletResult result) {
  if (result == PelletResult::Pellet) {
    score_ += 10;
  } else if (result == PelletResult::PowerPellet) {
    score_ += 50;
    ghostManager_->frighten();
  }

  if (maze_.pelletsRemaining() == 0) {
    state_ = State::LevelClear;
    stateTimer_ = 2.0f;
  }
}

void Game::onGhostEaten() { score_ += 200; }

void Game::onPacmanCaught() {
  if (state_ != State::Playing) {
    return;
  }
  state_ = State::PacmanDying;
  stateTimer_ = 1.5f;
  pacman_->setDead(true);
}

void Game::resetGame() {
  score_ = 0;
  lives_ = 3;
  level_ = 1;
  resetLevel();
}

void Game::resetLevel() {
  maze_.loadDefault();
  resetPositions();
  state_ = State::Ready;
  stateTimer_ = 2.0f;
  paused_ = false;
}

void Game::resetPositions() {
  pacman_->reset();
  ghostManager_->reset();
}
