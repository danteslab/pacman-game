#pragma once
#ifndef ACTOR_HPP
#define ACTOR_HPP

#include <raylib.h>

class Actor {
public:
  explicit Actor(float x = 0.0f, float y = 0.0f, float rotation = 0.0f);
  virtual ~Actor();

  virtual void draw() const = 0;
  virtual void update() = 0;

  Vector2 position;
  float rotation;
};

#endif // !ACTOR_HPP
