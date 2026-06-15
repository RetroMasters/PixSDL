# PixSDL

![PixSDL demo](Media/SceneLooping.gif)

Demo scene showing seamless scene looping, 2D-to-3D scene mapping, and animated sprite mesh deformation.

---

## Overview

PixSDL is a foundational sprite-based 2D/3D game engine core written in C++,
built on top of SDL2. It provides core systems such as input handling, audio,
texture handling, efficient sprite rendering, sprite animation, and a traditional
double-buffered game loop lifecycle.

---

## Design Philosophy

PixSDL assumes an SDL2-based environment, including SDL2 rendering, and follows
a traditional double-buffered game loop architecture. The core library is
single-threaded by design to keep behavior simple, predictable, and compatible
with SDL2 subsystems, while systems built on top are not conceptually limited to
a single-threaded model.

Within these constraints, the library is designed as a scalable foundation for
real-time applications, with a strong focus on flexibility, robustness, and
clarity.

The architecture is object-oriented, but favors small, composable building
blocks with clear and minimal responsibilities. The code aims to strike a
balance between being explicit and avoiding unnecessary visual noise. Explicit
casts are used where they improve type clarity, while modern C++ features are
used sparingly to preserve readability, portability, and wide compiler
compatibility.

---

## Build

- Download the repository
- Open `PixSDL.slnx` in Visual Studio 2026
- Build and run the project

The repository includes the required SDL2 development files.

---

## Requirements

- Visual Studio 2026
- C++14
- SDL2
- SDL2_image
- SDL2_ttf
- SDL2_mixer
 
---

## License

PixSDL is released under the MIT License.

---

## Author

PixSDL is developed by RetroMasters.  
Real name: Juri Schupilo
