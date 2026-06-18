# PixSDL

![PixSDL demo](Media/SceneLooping.gif)

Demo showing seamless scene looping, 2D-to-3D scene mapping, and animated mesh deformation.

---

## Overview

PixSDL is a sprite-based 2D/3D game engine core written in C++ and built on top
of SDL2. The repository is currently provided as a runnable Visual Studio project
so the engine code can be explored, modified, and tested directly.

It provides core systems such as modular input handling, audio, texture
handling, sprite animation, sprite rendering with motion interpolation, and a
double-buffered game loop with stable pacing.

PixSDL is designed as a foundation for:

- Traditional pixel-art games
- 2D/3D Super Scaler-style games
- Sprite-based games with large coordinate spaces
- Simulation-oriented sandbox games with texture-rendered pixel worlds at traditional pixel-art resolutions
- A fantasy-console-style rendering API for direct game development workflows

---

## Design Philosophy

PixSDL is built around SDL2 and uses SDL2's renderer API as its rendering
backend. The library provides a stable double-buffered game loop framework,
while also exposing modular building blocks that can be combined at different
levels. It is single-threaded by design to keep behavior simple and compatible
with SDL2 subsystems, while systems built on top of PixSDL are not generally
limited to a single-threaded model.

Within these constraints, the library is designed as a scalable foundation for
real-time applications, with a strong focus on flexibility, robustness, and
clarity.

The architecture is object-oriented, but favors small, composable building
blocks with clear and minimal responsibilities. The code aims to strike a
balance between being explicit and avoiding unnecessary visual noise. C++
language features are chosen and used deliberately to preserve readability,
portability, and wide compiler compatibility.

---

## Build

- Download the repository
- Open `PixSDL.slnx` in Visual Studio 2026
- Build and run the project

The repository includes the required SDL2 development files.

---

## Requirements

- Visual Studio 2026 with C++ desktop development support
  
---

## License

PixSDL is released under the MIT License.

---

## Author

PixSDL is developed by RetroMasters.  
Real name: Juri Schupilo
