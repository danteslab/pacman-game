#pragma once
#ifndef MAZE_RENDERER_HPP
#define MAZE_RENDERER_HPP

#include "Actor.hpp"

class Maze;

class MazeRenderer : public Actor {
public:
  explicit MazeRenderer(Maze *maze);
  ~MazeRenderer() override;

  void draw() const override;
  void update() override;

private:
  Maze *maze_;
};

#endif // !MAZE_RENDERER_HPP
