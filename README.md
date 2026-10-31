# Pac-Man

A Pac-Man implementation in C++17 using [raylib](https://www.raylib.com/).

The engine architecture is adapted from the [Flappy Bird](https://github.com/dantehemerson/flappy-bird)
project: an `Application` drives a `Game`, which owns managers and actors. The game layer
implements the Pac-Man rules.

## About

Pac-Man moves through a 28×31 tile maze, eating pellets and power pellets while avoiding four
ghosts. Power pellets turn the ghosts blue for 7 seconds; touching a blue ghost sends it back
to the house. Clear all pellets to advance to the next level.

## Controls

| Input | Action |
|-------|--------|
| Arrow keys / WASD | Move Pac-Man |
| P | Pause / unpause |
| Enter | Restart after game over |
| Esc | Quit |

## Requirements

- A C++17 compiler (`clang++` by default; override with `CXX=...`)
- [raylib](https://www.raylib.com/) discoverable through `pkg-config`
- On macOS, the link step uses the OpenGL, Cocoa, IOKit, CoreAudio, AudioToolbox, and
  CoreVideo frameworks

## Quick Start

```bash
./run.sh
```

Or build and run manually:

```bash
make
./pacman
```

Other targets:

```bash
make run     # build and run
make clean   # remove ./pacman
```

## Project Structure

```
main.cpp                  Application entry point
src/engine/               Reusable engine layer
  Application             window, audio, FPS, main loop
  Actor                   drawable/updatable entity
  ActorManager            owns actors and updates them
  StageManager            draw pass in insertion order
  Control / Peripheral    input bindings and devices
  Direction / TileCoord   grid value types and coordinate helpers
  Globals                 tile size, maze size, window settings
src/game/                 Pac-Man rules and rendering
  Game                    orchestration and state machine
  Maze                    ASCII maze data and pellet state
  GridMover               tile-center movement and turning
  PacMan                  player actor
  Ghost / GhostManager    ghost AI, modes, and collisions
  MazeRenderer            wall and pellet rendering
  Hud                     score, level, and lives
```

`Game` registers actors with `ActorManager` in draw order: `MazeRenderer`, `GhostManager`,
`PacMan`, `Hud`.

## How It Works

`Application` opens the window and runs the loop. Each frame it calls `Game::draw()` and
`Game::update()`. `Game` owns an `ActorManager` (actor lifetime and updates), a `StageManager`
(draw pass), and a `ControlManager` (input bindings).

`GridMover` handles tile-based movement. Entities move along one axis, snap to tile centers, and
turn when the next tile is walkable. Reversals are immediate; other turns wait for the next
center. Movement is frame-based at the 60 FPS target.

`Maze` parses an embedded ASCII map (`#` wall, `.` pellet, `o` power pellet, `-` ghost door,
`T` tunnel, `P` Pac-Man spawn, `G` ghost spawn, `H` ghost house). Walkability, pellet
consumption, and horizontal tunnel wrapping live there.

## Current Status

- Builds clean with `-Wall -Wextra`.
- Maze parses to 28×31 with 244 pellets, four ghost spawns, a door, and a house.
- Pac-Man moves tile-by-tile, turns at centers, stops at walls, and wraps tunnels.
- Ghosts, pellets, power pellets, collisions, lives, level flow, pause, and restart are wired.

Planned:

- Sprite atlas and animated sprites
- Sound effects
- WebAssembly build and GitHub Pages deployment
- Automated headless tests
- Fruit, ghost-house release timers, and per-level difficulty

## Contributing

Contributions, issues, and feature requests are welcome. Keep engine code in `src/engine/` free
of Pac-Man rules; game-specific behavior belongs in `src/game/`. Commit messages follow
Conventional Commits (`feat:`, `fix:`, `chore:`).
