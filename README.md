# 🛡️ DIU Under Siege: The Certified Cow Disaster Simulation 🐄💥

[![C++11](https://img.shields.io/badge/Language-C%2B%2B11-00599C?style=for-the-badge&logo=c%2B%2B)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/Graphics-OpenGL-5586A4?style=for-the-badge&logo=opengl)](https://www.opengl.org/)
[![GLFW](https://img.shields.io/badge/Window-GLFW3-orange?style=for-the-badge)](https://www.glfw.org/)
[![Audio](https://img.shields.io/badge/Audio-miniaudio-blueviolet?style=for-the-badge)](https://miniaud.io/)
[![Platform](https://img.shields.io/badge/Platform-macOS-black?style=for-the-badge&logo=apple)](https://www.apple.com/macos/)
[![License: Fun Only](https://img.shields.io/badge/License-Fun_&_Personal_Use_Only-orange?style=for-the-badge)](LICENSE)

> An interactive 2D procedural destruction engine, particle physics simulator, and tactical strike game developed in C++ and OpenGL.

---

## 📖 Overview

**DIU Under Siege** simulates tactical disaster strikes on the iconic university campus building while blending serious classical computer graphics algorithms with lighthearted campus humor.

From aerodynamic cruise missile trajectories and high-altitude drone bombing runs to ground-shattering seismic shocks, the environment reacts dynamically with real-time mesh slicing, multi-stage particle physics, and damped harmonic screen shakes. As a campus inside-joke, students and the beloved campus cows roam the greens — with a playable **Certified Cow** sporting a university student ID card!

---

## 🌟 Key Features

### 🚀 1. Multi-Hazard Tactical Strikes
* **Cruise Missile Strike (`M`):** Aerodynamic projectile featuring thrust, atmospheric drag, trajectory tracking, continuous smoke trails, and explosive impact upon contact.
* **Drone Bomber (`D`):** Aerial drone with bobbing flight mechanics and animated rotating propellers that releases a ballistic free-fall bomb over the target zone.
* **Seismic Charge / Earthquake (`E`):** Low-frequency tremor warnings triggering intensive screen rumbles, ground shockwaves, and structural subsidence.

### 🐄 2. The "Certified Cow" & Campus Life
* **Playable Student Cow (`C`):** Take control of an articulated cow wearing a DIU Student ID card!
* **Interactive Behaviors:** Walk across campus fields with directional tilt, graze grass (`G`), moo on command, and react dynamically to incoming attacks.
* **Panic AI & Evacuation:** Both stick-figure students (NPCs) and campus cows transition from idle wander mode to high-speed panic evasion when launch alarms sound.

### 💥 3. Structural Destruction & Dynamic Physics
* **100-Piece UV-Mapped Fragmentation:** On impact, the campus building fractures dynamically into a $10 \times 10$ grid of individual textured blocks (`Fragments`).
* **Rigid Body Physics:** Each fragment inherits radial explosive impulses, angular momentum (spin), gravity acceleration, and ground restitution (bouncing and friction).
* **Continuous Fire & Secondary Explosions:** Post-impact fires spread across damaged structures and foliage with upward buoyant particles.

### 🎆 4. Multi-Stage Particle System
* **Smoke & Exhaust Trails:** Expanding alpha-blended smoke clouds with velocity damping.
* **Fireballs & High-Speed Sparks:** Radiating high-temperature particles with randomized lifespans and color degradation (yellow $\rightarrow$ orange $\rightarrow$ red).
* **Expanding Shockwaves:** Expanding circular shock rings generated using the Midpoint Circle Algorithm.
* **Ground Dust & Seismic Clouds:** Dense dirt particulate clouds for earthquake simulations.

### 🔊 5. Integrated Sound Engine
* Powered by `miniaudio`, featuring layered ambient background music, alarm sirens, launch whooshes, missile detonations, bomb drops, seismic rumbles, and cow moos.

---

## 📐 Core Computer Graphics Algorithms Implemented

| Algorithm / Technique | Code Implementation | Description & Usage |
| :--- | :--- | :--- |
| **DDA Line Drawing** | [`drawLineDDA()`](graphics.cpp) | Custom Digital Differential Analyzer line rasterization simulating pixel grids to render stick-figure student limbs and torsos. |
| **Midpoint Circle Algorithm** | [`drawCircleMidpoint()`](graphics.cpp) | Bresenham's Midpoint Circle algorithm supporting both outlined and filled modes for human heads, projectile tips, and shockwave rings. |
| **Hierarchical 2D Skeletal Animation** | [`drawProceduralNPC()`](entities.cpp) & [`drawProceduralCow()`](entities.cpp) | Forward kinematics utilizing the OpenGL matrix stack (`glPushMatrix`, `glRotatef`, `glTranslatef`) to animate articulated walking legs, swinging arms, bobbing heads, and lanyards. |
| **7-Segment Procedural Display** | [`draw7SegmentDigit()`](graphics.cpp) | Pure vector line-based 7-segment digital display for launch countdown numbers ($3 \dots 2 \dots 1$). |
| **UV Texture Slicing** | [`initFragments()`](entities.cpp) | Real-time calculation of normalized UV coordinates ($tx, ty, tw, th$) mapped to fragmented polygon quads. |
| **Damped Screen Shake** | [`update()`](game.cpp) | Harmonic oscillation damped exponentially: $\Delta = \sin(\omega t) \cdot M \cdot e^{-\lambda t}$ |

---

## 🎮 Controls & Keybindings

| Key | Action | Details |
| :---: | :--- | :--- |
| <kbd>M</kbd> | **Cruise Missile Strike** | Triggers launch countdown, siren, trajectory, and impact. |
| <kbd>D</kbd> | **Drone Bomber** | Deploys drone overhead to drop a ballistic gravity bomb. |
| <kbd>E</kbd> | **Earthquake** | Activates seismic tremor, screen shake, and building collapse. |
| <kbd>C</kbd> | **Spawn / Control Cow** | Spawns the playable DIU Certified Cow with student ID. |
| <kbd>↑</kbd> <kbd>↓</kbd> <kbd>←</kbd> <kbd>→</kbd> | **Move Cow** | Steers the cow across the campus terrain. |
| <kbd>G</kbd> | **Eat Grass / Moo** | Triggers eating animation and cow moo sound effect. |
| <kbd>Space</kbd> | **Reset Scene** | Restores building, clears particles, and resets agents. |
| <kbd>S</kbd> | **Mute / Unmute** | Toggles background theme music. |
| <kbd>H</kbd> | **Help / Controls HUD** | Hold to display on-screen control keyboard buttons. |
| <kbd>Esc</kbd> | **Quit** | Exits application cleanly. |

---

## 📂 Project Architecture

```
ComputerGraphic_Project/
├── main.cpp              # Application entry, GLFW window loop, glOrtho setup & key callbacks
├── game.cpp              # Game update loop, attack state machine, HUD rendering, screen shake
├── game.h                # Game module declarations
├── entities.cpp          # Procedural NPC/Cow drawing, particle physics, mesh fragmentation
├── entities.h            # Entity functions and structures
├── graphics.cpp          # DDA line, Midpoint circle, 7-Segment digit, texture loaders
├── graphics.h            # Graphics definitions and GLUT font utilities
├── config.h              # World bounds, physics constants, entity data structs
├── state.cpp             # Global state variables and texture IDs
├── state.h               # State declarations
├── audio.cpp             # Audio engine wrapper using miniaudio
├── audio.h               # Audio handles and function headers
├── build.sh              # Single-command compilation script for macOS
├── dependencies/
│   ├── include/          # GLFW, stb_image, and miniaudio headers
│   └── library/          # macOS dynamic libraries (libglfw.3.dylib)
├── images/               # Campus before/after textures and projectile sprites
└── audio/                # High-fidelity audio wave assets
```

---

## 🛠️ Build & Run Instructions

### Prerequisites (macOS)
* **Clang / LLVM** with C++11 support (`xcode-select --install`)
* **OpenGL Framework** & **GLUT Framework** (Pre-installed natively on macOS)
* Dynamic GLFW library is already bundled inside `dependencies/library/`

### Compilation

Clone the repository and run the build script:

```bash
# 1. Clone repository
git clone git@github.com:methun404/diu-siege-certified-cow.git
cd diu-siege-certified-cow

# 2. Make build script executable and compile
chmod +x build.sh
./build.sh

# 3. Launch the simulation
./app
```

---

## 📄 License

This project is licensed under a **Non-Commercial Fun & Personal Use License**.

* 🎉 **Permitted:** Free to play, run, study, modify, and share strictly for personal fun, humor, learning, and entertainment.
* 🚫 **Restricted:** Strictly no commercial use, monetization, or selling of this project or its assets.

See the [LICENSE](LICENSE) file for full terms and conditions.

