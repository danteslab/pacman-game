#include "Application.hpp"

#include <raylib.h>

#include "Game.hpp"
#include "Globals.hpp"
#include "Keyboard.hpp"

Application::Application() : exit_(false), game_(nullptr), keyboard_(nullptr) {
  settings = {Globals::Settings::WIDTH, Globals::Settings::HEIGHT, Globals::Settings::FPS,
              Globals::SCALE, "Pac-Man"};
}

Application::~Application() {
  delete game_;
  delete keyboard_;
  CloseAudioDevice();
  CloseWindow();
}

void Application::init(int argc, char **argv) {
  (void)argc;
  (void)argv;

  InitWindow(settings.width, settings.height, settings.title.c_str());
  InitAudioDevice();
  SetTargetFPS(settings.fps);

  keyboard_ = new Keyboard();
  game_ = new Game(this);
}

void Application::loop() {
  while (!exit_ && !WindowShouldClose()) {
    if (IsKeyPressed(KEY_ESCAPE)) {
      break;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    game_->draw();
    game_->update();

    EndDrawing();
  }
}

void Application::quit() { exit_ = true; }

Keyboard *Application::getKeyboard() const { return keyboard_; }
