<div align="center">

# 💎 Jewels Puzzle in C  
**A Match‑3 style puzzle game in C featuring gravity control, scoring, save/load, and an automatic puzzle solver (BFS/DFS/A\*).**

<img src="assets/preview.gif" width="760" alt="Gameplay preview"/>

<p>
  <img src="https://img.shields.io/badge/language-C-blue" alt="language badge"/>
  <img src="https://img.shields.io/badge/build-make-success" alt="build badge"/>
  <img src="https://img.shields.io/badge/solver-BFS%20%7C%20DFS%20%7C%20A*-7b2cbf" alt="solver badge"/>
  <img src="https://img.shields.io/badge/status-active-orange" alt="status badge"/>
</p>

</div>

---

## 📚 Table of Contents
- [✨ Features](#-features)
- [🧪 Usage](#-usage)
- [🧩 Puzzle Solver](#-puzzle-solver)
- [💾 Save & Load](#-save--load)
- [🏆 Scores](#-scores)
- [🧱 Project Structure](#-project-structure)
- [🛠️ Build](#️-build)
- [🧭 Roadmap](#-roadmap)
- [🤝 Contributing](#-contributing)
- [📜 License](#-license)

---

## ✨ Features
- ✅ Classic gameplay: swap, match, cascades
- 🌀 Gravity control: **UP / RIGHT / DOWN / LEFT**
- 💡 Hint system (suggests a valid move)
- 🧩 Puzzle mode + **automatic solver** (**BFS / DFS / A\***)
- 💾 Save & load game states
- 🏆 Score table: load / save / sort by score metric

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 🧪 Usage

### Controls *(adapt if your GUI labels differ)*
- 🖱️ **Swap**: click two adjacent cells to swap them
- 💥 **Detonation**: triggers elimination of detected matches
- 🌀 **Change gravity**: shifts the board (UP / RIGHT / DOWN / LEFT)
- 💡 **Hint**: highlights a possible move

### Visuals
<p align="center">
  <img src="assets/classic.gif" width="420" alt="Classic mode demo"/>
  <img src="assets/solver.gif" width="420" alt="Solver demo"/>
</p>

> **GIF tip:** keep assets under ~10MB for faster loading on GitHub.

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 🧩 Puzzle Solver

The solver models the game as a **graph of configurations**:

- **BFS (Breadth‑First Search)**  
  Finds the **shortest** solution, but can use a lot of memory.
- **DFS (Depth‑First Search)**  
  Often finds a solution quickly but not necessarily optimal (may be long).
- **A\***  
  Uses a heuristic to prioritize promising states (typically best balance).

### Heuristic used in A\*
✅ **h(config) = number of non‑empty cells**

### Example
```txt
Solve method: A*
Moves found: 12
Configurations explored: 1840
Result: SOLVED ✅
```

<details>
  <summary><b>🧠 How it works (click)</b></summary>

Each distinct board configuration is treated as a node in a graph.  
A player action (swap, gravity change, cycle, storage action) generates a transition to another configuration.

The solver explores configurations starting from the initial state until reaching a **goal state**:
✅ a configuration where the board becomes **empty**.

To prevent revisiting already-seen states, explored configurations are stored in a **hash table**.

</details>

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 💾 Save & Load

You can save and reload a game state.

### Save
```bash
./projet --save saves/mygame.save
```

### Load
```bash
./projet --load saves/mygame.save
```

> Edit these commands to match your actual CLI / GUI workflow.

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 🏆 Scores

Scores are stored in a file (example: `scores.txt`) and support:
- loading existing score tables
- saving updated tables (including a new player entry)
- sorting by score type (global, level, swaps, etc.)

### Example line format
```txt
PlayerName;1200;800;12;45;0;0;2;3;150
```

<details>
  <summary><b>📌 About score fields (click)</b></summary>

The score columns typically map to your `score_type` enum, for example:
- Global score
- Level score
- Swaps
- Consumed gems
- Flamed / Starred / Hypercubes
- Gravity actions
- Time (if enabled)

Adjust the mapping description if your implementation differs.

</details>

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 🧱 Project Structure
```txt
Source_code/
├── game_play.c / game_play.h        # Gameplay + animations + scoring
├── game_solver.c / game_solver.h    # Puzzle solver (BFS/DFS/A*)
├── game_scores.c / game_scores.h    # Score load/save/sort
├── game_save.c / game_save.h        # Save/load game state
├── gui_*                            # GUI rendering + menus + controller
└── Makefile
assets/
├── preview.gif
├── classic.gif
└── solver.gif
```

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 🛠️ Build

### Requirements
- A C compiler (e.g., `gcc` or `clang`)
- `make`

### Build
```bash
make
```

### Debug build
```bash
make DEBUG=1
```

### Clean
```bash
make clean
```

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 🧭 Roadmap
Planned gameplay upgrades (aligned with your specification):

- 🔷 **Hypercubes**
  - Match 5+ → promote the center gem to a hypercube
  - Swap hypercube with a gem → remove all gems of that type (+ hypercube)
  - Swap two hypercubes → remove all hypercubes on the board
  - If the promoted gem had a power → trigger it (no hypercube created)

- 🪨 **Rocks**
  - Blockers with no active effect
  - Destroyable only via explosions or hypercube effects

- 💣 **Bombs**
  - Countdown decreases each player action
  - When countdown reaches 0 → explodes on next action
  - Explosion removes a 3×3 area (like a flame explosion)

- 🎨 Improved animations & visual feedback
- 📦 More puzzles + puzzle generator
- 🧠 Stronger heuristics for A*

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 🤝 Contributing
Contributions are welcome.

1. Fork the repository  
2. Create a feature branch:
   ```bash
   git checkout -b feature/my-feature
   ```
3. Commit changes:
   ```bash
   git commit -m "Add: my feature"
   ```
4. Push:
   ```bash
   git push origin feature/my-feature
   ```
5. Open a Pull Request ✅

<p align="right">(<a href="#-table-of-contents">back to top</a>)</p>

---

## 📜 License
This project is licensed under the **MIT License** *(update if different).*

---

<div align="center">

⭐ If you like this project, please consider giving it a star!

<img src="assets/star.gif" width="200" alt="Star gif"/>

</div>
