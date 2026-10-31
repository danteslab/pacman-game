#include "MazeRenderer.hpp"

#include <raylib.h>

#include "Globals.hpp"
#include "Maze.hpp"
#include "TileCoord.hpp"

MazeRenderer::MazeRenderer(Maze *maze) : maze_(maze) {}

MazeRenderer::~MazeRenderer() {}

void MazeRenderer::update() {}

void MazeRenderer::draw() const {
  float tile = static_cast<float>(Globals::TILE_SIZE);

  for (int row = 0; row < maze_->rows(); ++row) {
    for (int col = 0; col < maze_->cols(); ++col) {
      TileCoord coord{col, row};
      TileType type = maze_->at(coord);
      Vector2 center = centerOf(coord);

      switch (type) {
        case TileType::Wall: {
          Rectangle rect{col * tile, static_cast<float>(Globals::MAZE_OFFSET_Y) + row * tile, tile,
                         tile};
          DrawRectangleRec(rect, Color{16, 16, 64, 255});
          DrawRectangleLinesEx(rect, 1.0f, Color{40, 60, 200, 255});
          break;
        }
        case TileType::Pellet:
          DrawCircleV(center, tile * 0.08f, RAYWHITE);
          break;
        case TileType::PowerPellet:
          DrawCircleV(center, tile * 0.20f, RAYWHITE);
          break;
        case TileType::GhostDoor:
          DrawRectangle(static_cast<int>(col * tile),
                        static_cast<int>(Globals::MAZE_OFFSET_Y + row * tile + tile * 0.4f),
                        static_cast<int>(tile), static_cast<int>(tile * 0.2f), PINK);
          break;
        default:
          break;
      }
    }
  }
}
