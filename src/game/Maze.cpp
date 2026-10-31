#include "Maze.hpp"

#include <sstream>

static const char *kDefaultMaze = R"(############################
#............##............#
#.####.#####.##.#####.####.#
#o####.#####.##.#####.####o#
#.####.#####.##.#####.####.#
#..........................#
#.####.##.########.##.####.#
#.####.##.########.##.####.#
#......##....##....##......#
######.##### ## #####.######
     #.##### ## #####.#     
     #.##          ##.#     
     #.## ###--### ##.#     
######.## #      # ##.######
      .   #GG HGG#   .      
######.## #      # ##.######
     #.## ######## ##.#     
     #.##          ##.#     
     #.## ######## ##.#     
######.## ######## ##.######
#............##............#
#.####.#####.##.#####.####.#
#.####.#####.##.#####.####.#
#o..##.......P .......##..o#
###.##.##.########.##.##.###
###.##.##.########.##.##.###
#......##....##....##......#
#.##########.##.##########.#
#.##########.##.##########.#
#..........................#
############################)";

Maze::Maze() : cols_(0), rows_(0), pelletsRemaining_(0) {}

bool Maze::loadDefault() { return loadFromText(kDefaultMaze); }

bool Maze::loadFromText(const std::string &text) {
  std::istringstream stream(text);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(stream, line)) {
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (line.empty()) {
      continue;
    }
    lines.push_back(line);
  }

  if (lines.empty()) {
    return false;
  }

  cols_ = static_cast<int>(lines.front().size());
  rows_ = static_cast<int>(lines.size());
  tiles_.assign(static_cast<size_t>(cols_ * rows_), TileType::Empty);
  pelletsRemaining_ = 0;
  pacmanSpawn_ = {1, 1};
  ghostHouse_ = {1, 1};
  ghostDoor_ = {1, 1};
  ghostSpawns_.clear();

  for (int row = 0; row < rows_; ++row) {
    const std::string &current = lines[static_cast<size_t>(row)];
    for (int col = 0; col < cols_; ++col) {
      char symbol = col < static_cast<int>(current.size()) ? current[static_cast<size_t>(col)] : ' ';
      TileCoord tile{col, row};

      switch (symbol) {
        case '#':
          set(tile, TileType::Wall);
          break;
        case '.':
          set(tile, TileType::Pellet);
          ++pelletsRemaining_;
          break;
        case 'o':
          set(tile, TileType::PowerPellet);
          ++pelletsRemaining_;
          break;
        case '-':
          if (ghostDoor_ == TileCoord{1, 1}) {
            ghostDoor_ = tile;
          }
          set(tile, TileType::GhostDoor);
          break;
        case 'T':
          set(tile, TileType::Tunnel);
          break;
        case 'P':
          pacmanSpawn_ = tile;
          set(tile, TileType::Empty);
          break;
        case 'H':
          ghostHouse_ = tile;
          set(tile, TileType::Empty);
          break;
        case 'G':
          ghostSpawns_.push_back(tile);
          set(tile, TileType::Empty);
          break;
        default:
          set(tile, TileType::Empty);
          break;
      }
    }
  }

  return true;
}

int Maze::cols() const { return cols_; }

int Maze::rows() const { return rows_; }

bool Maze::inBounds(TileCoord tile) const {
  return tile.col >= 0 && tile.col < cols_ && tile.row >= 0 && tile.row < rows_;
}

TileType Maze::at(TileCoord tile) const {
  if (!inBounds(tile)) {
    return TileType::Wall;
  }
  return tiles_[static_cast<size_t>(tile.row * cols_ + tile.col)];
}

void Maze::set(TileCoord tile, TileType type) {
  if (!inBounds(tile)) {
    return;
  }
  tiles_[static_cast<size_t>(tile.row * cols_ + tile.col)] = type;
}

bool Maze::isWalkable(TileCoord tile, bool canUseDoor) const {
  TileCoord wrapped = wrap(tile);
  if (!inBounds(wrapped)) {
    return false;
  }

  TileType type = at(wrapped);
  if (type == TileType::Wall) {
    return false;
  }
  if (type == TileType::GhostDoor) {
    return canUseDoor;
  }
  return true;
}

TileCoord Maze::wrap(TileCoord tile) const {
  if (cols_ <= 0) {
    return tile;
  }
  tile.col = ((tile.col % cols_) + cols_) % cols_;
  return tile;
}

PelletResult Maze::consume(TileCoord tile) {
  if (!inBounds(tile)) {
    return PelletResult::None;
  }

  TileType &type = tiles_[static_cast<size_t>(tile.row * cols_ + tile.col)];
  if (type == TileType::Pellet) {
    type = TileType::Empty;
    --pelletsRemaining_;
    return PelletResult::Pellet;
  }
  if (type == TileType::PowerPellet) {
    type = TileType::Empty;
    --pelletsRemaining_;
    return PelletResult::PowerPellet;
  }
  return PelletResult::None;
}

int Maze::pelletsRemaining() const { return pelletsRemaining_; }

TileCoord Maze::pacmanSpawn() const { return pacmanSpawn_; }

const std::vector<TileCoord> &Maze::ghostSpawns() const { return ghostSpawns_; }

TileCoord Maze::ghostHouse() const { return ghostHouse_; }

TileCoord Maze::ghostDoor() const { return ghostDoor_; }
