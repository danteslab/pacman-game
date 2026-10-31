# AGENTS.md

Guidance for agents and contributors working in this repository.

## What this is

A Pac-Man implementation in C++17 using [raylib](https://www.raylib.com/). The engine
architecture is adapted from the Flappy Bird project at `/Users/d/coding/games/flappy-bird`:
an `Application` drives a `Game`, which owns managers and actors. The game layer is specific to
Pac-Man.

## Commands

```bash
make            # build ./pacman
make run        # build and run
./run.sh        # build and run
make clean      # remove ./pacman
```

- Compiler: `clang++` (override with `CXX=...`).
- Flags: `-std=c++17 -Wall -Wextra -O2`.
- raylib is discovered with `pkg-config`; on macOS the link step also adds OpenGL/Cocoa/IOKit/
  CoreAudio/AudioToolbox/CoreVideo frameworks.
- There is no test suite yet. Verify changes by building and running the game. For logic-only
  changes, a temporary harness can compile `src/game/Maze.cpp` and `src/game/GridMover.cpp`
  without opening a window.

## Layout

```
main.cpp                  Application entry point
src/engine/               Reusable engine layer
  Application             window, audio, FPS, main loop, owns Game + Keyboard
  Interface               base for objects that receive Application* and have draw/update
  Actor                   drawable/updatable entity (position, rotation)
  ActorManager            owns actors, updates them, draws all in insertion order
  StageManager            draw pass over ActorManager
  ControllableObject      doAction(action, magnitude) interface
  ControllableActor       Actor + ControllableObject
  Peripheral              input device interface
  Keyboard                raylib keyboard peripheral
  Control                 binds action -> peripheral component + event
  ControlManager          owns Controls, updates them each frame
  Direction               Direction enum + vector/opposite helpers
  TileCoord               TileCoord, tileOf(pixel), centerOf(tile)
  Globals                 tile size, maze dimensions, window settings
src/game/                 Pac-Man rules and rendering
  Maze                    ASCII maze data, walkability, pellet consumption, spawns
  GridMover               tile-center movement and turning
  PacMan                  player actor
  Ghost                   ghost actor and targeting
  GhostManager            owns ghosts, scatter/chase waves, frightened timer, collisions
  MazeRenderer            draws walls, pellets, power pellets, ghost door
  Hud                     score, level, lives
  Game                    orchestration, state machine, scoring, level flow
```

`Game` registers actors with `ActorManager` in this order: `MazeRenderer`, `GhostManager`,
`PacMan`, `Hud`. Insertion order is draw order; later actors draw on top.

## Coordinate system

- Logical tile size: `Globals::TILE_SIZE` = 24 px (`TILE_BASE` 8 × `SCALE` 3).
- Maze: 28 columns × 31 rows.
- HUD reserves the top 2 rows. Maze row 0 starts at `Globals::MAZE_OFFSET_Y`.
- Window: 672 × 792.
- Always convert with `tileOf(pixel)` and `centerOf(tile)` from `TileCoord.hpp`; do not
  hand-roll the HUD offset.

## Maze format

`Maze::loadDefault()` parses an embedded ASCII map in `src/game/Maze.cpp`:

| Symbol | Meaning |
|--------|---------|
| `#` | wall |
| `.` | pellet |
| `o` | power pellet |
| `-` | ghost house door (ghosts only) |
| `T` | tunnel |
| `P` | Pac-Man spawn |
| `G` | ghost spawn |
| `H` | ghost house center |

Rows must all be the same width (28). Add or change markers rather than hard-coding spawn
coordinates. `Maze::wrap` handles horizontal tunnel wrapping.

## Movement model

`GridMover` moves an entity along one axis and snaps to tile centers:

- At a center it applies the desired direction if walkable, otherwise keeps or stops the
  current direction.
- Reversal is immediate; other turns wait for the next tile center.
- Movement is frame-based (pixels per frame at the 60 FPS target), not delta-time.
- Speeds: Pac-Man `TILE_SIZE / 8`, ghost `TILE_SIZE / 10`, frightened `TILE_SIZE / 14`,
  eaten `TILE_SIZE / 5`.
- `canUseDoor` is false for Pac-Man and true for ghosts that are leaving the house or eaten.

## Game rules

- States: `Ready`, `Playing`, `PacmanDying`, `LevelClear`, `GameOver`.
- Ready lasts 2 s. Death lasts 1.5 s. Level clear lasts 2 s.
- Pellets score 10; power pellets score 50 and frighten ghosts for 7 s; ghosts score 200.
- Lives start at 3. Level clear advances the level and reloads the maze.
- Ghost modes alternate scatter (7 s) and chase (20 s). Frightened overrides both.
- Ghost targeting: Blinky targets Pac-Man; Pinky 4 tiles ahead; Inky 2 tiles ahead
  (simplified, no Blinky-vector doubling); Clyde chases when farther than 8 tiles, otherwise
  goes to its scatter corner.
- Collision uses a 0.6-tile radius.

## Input

Controls are bound in the `Game` constructor:

| Input | Action |
|-------|--------|
| Arrows / WASD | move Pac-Man |
| P | pause / unpause |
| Enter | restart after game over |
| Esc | quit |

Movement bindings use `Peripheral::ON_PRESSING`; one-shot bindings use `ON_PRESS`. To add
input, create a `Control`, set its owner, add bindings, and register it with `ControlManager`.

## Conventions

- Keep `src/engine/` free of Pac-Man rules. Game-specific behavior belongs in `src/game/`.
- Prefer value types (`Direction`, `TileCoord`) over raw pairs.
- Use `override` on virtual methods and `nullptr` over `NULL`.
- No comments unless they explain non-obvious intent.
- Match the existing style: 2-space indentation, braces on the same line, `snake_case_` for
  private members.
- Commit messages follow Conventional Commits (`feat:`, `fix:`, `chore:`).
- Do not commit the `pacman` binary; it is ignored.

## Current status

Implemented and verified:

- Builds clean with `-Wall -Wextra`.
- Runs without crashes; maze parses to 28×31 with 244 pellets, 4 ghost spawns, door and house.
- Pac-Man moves tile-by-tile, turns at centers, stops at walls, and wraps tunnels.
- Ghosts, pellets, power pellets, collisions, lives, level flow, pause, and restart are wired.

Deferred / known limitations:

- Rendering uses raylib primitives; no sprite atlas or sounds yet. The Flappy Bird
  `R`/`Sprite`/`Frame` resource layer was intentionally left out until assets exist.
- Physics is frame-based and assumes 60 FPS; no delta-time.
- Inky targeting is simplified; no fruit, ghost-house release timers, or per-level difficulty.
- No WebAssembly build script yet (the reference project has one under `scripts/`).
- No automated tests.

## Next steps

1. Playtest the loop and tune speeds, wave timers, and collision radii.
2. Add a sprite atlas and reintroduce an `R` resource registry plus `Sprite`/`Frame`.
3. Add sound effects for pellets, power pellets, ghost eaten, and death.
4. Port `scripts/build-wasm.sh`, `scripts/shell.html`, and the GitHub Pages workflow from the
   Flappy Bird reference project.
5. Add headless tests for `Maze` parsing, `GridMover` movement, and ghost targeting.
