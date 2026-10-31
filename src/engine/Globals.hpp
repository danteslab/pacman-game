#pragma once
#ifndef GLOBALS_HPP
#define GLOBALS_HPP

namespace Globals {

  constexpr int TILE_BASE = 8;
  constexpr int SCALE = 3;
  constexpr int TILE_SIZE = TILE_BASE * SCALE;

  constexpr int MAZE_COLS = 28;
  constexpr int MAZE_ROWS = 31;
  constexpr int HUD_ROWS = 2;
  constexpr int MAZE_OFFSET_Y = HUD_ROWS * TILE_SIZE;

  namespace Settings {
    constexpr int WIDTH = MAZE_COLS * TILE_SIZE;
    constexpr int HEIGHT = (MAZE_ROWS + HUD_ROWS) * TILE_SIZE;
    constexpr int FPS = 60;
  } // namespace Settings

} // namespace Globals

#endif // !GLOBALS_HPP
