#pragma once
#ifndef HUD_HPP
#define HUD_HPP

#include "Actor.hpp"

class Game;

class Hud : public Actor {
public:
  explicit Hud(Game *game);
  ~Hud() override;

  void draw() const override;
  void update() override;

private:
  Game *game_;
};

#endif // !HUD_HPP
