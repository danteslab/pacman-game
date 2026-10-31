#pragma once
#ifndef TILE_COORD_HPP
#define TILE_COORD_HPP

#include <cmath>
#include <raylib.h>

#include "Globals.hpp"

struct TileCoord {
  int col;
  int row;
};

inline bool operator==(TileCoord a, TileCoord b) { return a.col == b.col && a.row == b.row; }
inline bool operator!=(TileCoord a, TileCoord b) { return !(a == b); }

inline TileCoord tileOf(Vector2 pixel) {
  return {static_cast<int>(std::floor(pixel.x / Globals::TILE_SIZE)),
          static_cast<int>(std::floor((pixel.y - Globals::MAZE_OFFSET_Y) / Globals::TILE_SIZE))};
}

inline Vector2 centerOf(TileCoord tile) {
  return {tile.col * Globals::TILE_SIZE + Globals::TILE_SIZE / 2.0f,
          Globals::MAZE_OFFSET_Y + tile.row * Globals::TILE_SIZE + Globals::TILE_SIZE / 2.0f};
}

#endif // !TILE_COORD_HPP
