#include "Hud.hpp"

#include <raylib.h>

#include "Game.hpp"
#include "Globals.hpp"

Hud::Hud(Game *game) : game_(game) {}

Hud::~Hud() {}

void Hud::update() {}

void Hud::draw() const {
  DrawText(TextFormat("SCORE %d", game_->score()), 12, 8, 20, WHITE);
  DrawText(TextFormat("LEVEL %d", game_->level()), Globals::Settings::WIDTH / 2 - 40, 8, 20,
           WHITE);

  for (int i = 0; i < game_->lives(); ++i) {
    DrawCircle(Globals::Settings::WIDTH - 24 - i * 24, 18, 8, YELLOW);
  }
}
