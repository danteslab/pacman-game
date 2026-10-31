#pragma once
#ifndef MAZE_HPP
#define MAZE_HPP

#include <string>
#include <vector>

#include "TileCoord.hpp"

enum class TileType : char {
  Wall = '#',
  Pellet = '.',
  PowerPellet = 'o',
  Empty = ' ',
  GhostDoor = '-',
  Tunnel = 'T'
};

enum class PelletResult { None, Pellet, PowerPellet };

class Maze {
public:
  Maze();

  bool loadDefault();
  bool loadFromText(const std::string &text);

  int cols() const;
  int rows() const;
  bool inBounds(TileCoord tile) const;

  TileType at(TileCoord tile) const;
  bool isWalkable(TileCoord tile, bool canUseDoor) const;
  TileCoord wrap(TileCoord tile) const;

  PelletResult consume(TileCoord tile);
  int pelletsRemaining() const;

  TileCoord pacmanSpawn() const;
  const std::vector<TileCoord> &ghostSpawns() const;
  TileCoord ghostHouse() const;
  TileCoord ghostDoor() const;

private:
  void set(TileCoord tile, TileType type);

  std::vector<TileType> tiles_;
  int cols_;
  int rows_;
  int pelletsRemaining_;
  TileCoord pacmanSpawn_;
  std::vector<TileCoord> ghostSpawns_;
  TileCoord ghostHouse_;
  TileCoord ghostDoor_;
};

#endif // !MAZE_HPP
