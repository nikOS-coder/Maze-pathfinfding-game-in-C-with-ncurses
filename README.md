# C++ Terminal Maze Simulation & Autonomous Agents Game

An advanced, object-oriented console simulation written in **C++** utilizing the **ncurses** library. The program reads a custom maze layout from a file and runs an automated simulation where two autonomous character agents (`G` and `S`) navigate the maze, avoid hazards, pick up keys, rescue each other, and strive to meet and reach the exit ladder.

## Game Rules & Objectives

1. **The Ultimate Goal:** The two autonomous characters (`G` and `S`) must explore the maze and successfully **find and meet each other** (`X` and `Y` coordinates overlap).
2. **The Victory & Exit Sequence:** Once they meet, a "YOU WON!" message appears, the maze walls clear out dynamically, and both characters navigate collaboratively towards the final **Ladder (`L`)**.
3. **Hazards & Traps (`T`):** If a character steps on a trap, they become trapped inside a cage (`C`) and freeze in place.
4. **Game Over Conditions:** 
   - If **both** characters get trapped simultaneously.
   - If the maximum step limit (`termina == 1000`) is reached without achieving victory.

---

## Autonomous Navigation & Pathfinding Logic

Unlike traditional user-controlled games, this project implements **autonomous agent behavior** using Data Structures and Graph Traversal algorithms:

- **Depth-First Search (DFS) & Stacks:** Each character (`character` class) maintains a visited set (`std::set<pair<int, int>>`) and a movement stack (`std::set` / `std::stack`) to systematically explore unvisited adjacent cells (Up, Down, Left, Right).
- **Backtracking Mechanism:** When a character hits a dead end or encounters a cage without a key, the internal stack allows them to retrace their steps backward cleanly.
- **How They Find Each Other:** The main game loop updates both agents frame-by-frame (with smooth `ncurses` visual delays using `napms`). They explore independently until their positions intersect.

---

## Entities & Interactive Elements

- **`G` & `S` (Characters):** Autonomous agents navigating via DFS.
- **`K` (Key):** Essential item. If picked up by a character, it grants the ability to unlock/deactivate traps and free a trapped teammate.
- **`T` (Traps):** Hidden hazards that capture unwary characters.
- **`C` (Cage):** The visual state of a trapped character. Teammates equipped with the Key can approach and untrap them (`is_untrapped()`).
- **`L` (Ladder):** The final extraction point after the characters successfully meet.

---

## Tech Stack & Architecture

- **Language:** C++ (Standard C++11)
- **Terminal UI Library:** `ncurses` (Colors, raw/cbreak mode, cursor control, window coordinate queries)
- **Data Structures:** `std::stack`, `std::set`, dynamic 2D arrays (`char **`)
- **Object-Oriented Design:** Class inheritance hierarchy (`entity` base class -> `character`, `key`, `trap`, `ladder`)
- **Build System:** Makefile

---

## Prerequisites & Dependencies

To compile and run this application on a Linux/UNIX environment, ensure you have `g++` and the `ncurses` development library installed:

```bash
# Ubuntu / Debian / Mint
sudo apt-get install libncurses5-dev libncursesw5-dev

# Fedora / RHEL
sudo dnf install ncurses-devel
Compilation & Usage
This project includes a convenient Makefile for fast compilation.

1. Build the Project
Run make in the project directory:

Bash
make
2. Run the Game
Execute the binary by passing a maze text file as a command-line argument:

Bash
./maze_game maze_file.txt
3. Clean Build Files
To remove object files and binaries:

Bash
make clean
Project Structure
main.h / main.cpp — Handles file I/O, maze dimension parsing, ncurses color initialization, and the core game simulation loop.

entity.h / entity.cpp — Base abstract/virtual class managing coordinates, colors, and spawn validation.

character.h / character.cpp — Implements autonomous DFS movement, stacks, backtracking, and cage mechanics.

key.h / key.cpp — Key pickup and inventory logic.

trap.h / trap.cpp — Trap trigger and deactivation states.

ladder.h / ladder.cpp — Win coordinate tracking.

utils.h / utils.cpp — Screen character sampling helper functions via mvinch.

Makefile — Automated build script.

Author
Christopoulos Nikolas