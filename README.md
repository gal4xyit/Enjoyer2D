# Enjoyer2D

Enjoyer2D is an educational 2D physics engine written in C++17 with an interactive SDL3/Dear ImGui testbed. The project is built as a university physics engine demo: the engine code is separated from rendering and editor code so the physics logic can be tested, studied, and reused independently.

## Features

- 2D rigid bodies with box and circle shapes
- Static and dynamic bodies
- Gravity, force accumulation, velocity, mass, inverse mass
- Semi-implicit Euler integration with fixed timestep simulation
- Box-box, circle-circle, and box-circle collision detection
- Impulse-based collision response with restitution and friction
- Position correction for overlap reduction
- Configurable velocity and position solver iterations
- Runtime testbed with pause, reset, single-step, camera movement, and zoom
- Body spawning, selection, deletion, and live property editing
- Debug rendering for centers, velocity vectors, AABBs, contact points, contact normals, and motion paths
- Built-in demo scenes: free fall, bounce, stacking, friction, and stress testing
- JSON scene save/load support
- Catch2 test suite for core physics behavior

## Project Structure

```text
Enjoyer2D/
├── cmake/                 # Dependency and ImGui CMake helpers
├── engine/                # Physics engine library
│   └── include/Enjoyer2D/ # Public engine headers
├── testbed/               # SDL3 + Dear ImGui demo/editor application
│   └── src/
│       ├── app/           # Application lifecycle, input, update loop
│       ├── editor/        # ImGui UI and editor tools
│       ├── rendering/     # SDL_Renderer debug drawing and camera/view code
│       └── scenes/        # Built-in scenes and JSON scene serialization
├── tests/                 # Catch2 physics tests
├── vendor/imgui/          # Vendored Dear ImGui sources
└── saved_scenes/          # User-created or bundled JSON scenes
```

## Dependencies

Most dependencies are fetched automatically by CMake:

- SDL3
- GLM
- Catch2
- nlohmann/json

Dear ImGui is vendored in `vendor/imgui`.

You only need a C++17 compiler, CMake, and Git. On Windows, install Visual Studio with the **Desktop development with C++** workload.

## Build

Clone the repository:

```bash
git clone https://github.com/gal4xyit/Enjoyer2D.git
cd Enjoyer2D
```

Configure and build:

```bash
cmake -S . -B build
cmake --build build --target enjoyer2d_testbed
```

Run the testbed:

```bash
./build/testbed/enjoyer2d_testbed
```

On Windows, the executable is usually located in a configuration subfolder, for example:

```bat
build\testbed\Debug\enjoyer2d_testbed.exe
```

CMake copies required runtime DLLs next to the testbed executable on Windows.

## Running Tests

Build and run the Catch2 test suite:

```bash
cmake --build build --target enjoyer2d_tests
ctest --test-dir build --output-on-failure
```

## Building on Windows

Recommended setup:

- Visual Studio 2022 or newer
- **Desktop development with C++** workload
- CMake support for Visual Studio
- Git

It is recommended to clone the project into a simple ASCII-only path, for example:

```bat
C:\Dev\Enjoyer2D
```

Avoid paths with Cyrillic characters or OneDrive desktop folders if possible, because some third-party C/C++ build tools can behave badly with non-ASCII build paths.

### Option 1: Visual Studio CMake Project

1. Open Visual Studio.
2. Select **File > Open > Folder...**
3. Choose the `Enjoyer2D` project folder.
4. Wait until CMake finishes configuring the project.
5. Select the `enjoyer2d_testbed.exe` target.
6. Build and run it from Visual Studio.

The first configure/build can take some time because CMake downloads SDL3, GLM, Catch2, and nlohmann/json.

### Option 2: Developer Command Prompt

Open **Developer Command Prompt for Visual Studio** and run:

```bat
git clone https://github.com/gal4xyit/Enjoyer2D.git C:\Dev\Enjoyer2D
cd C:\Dev\Enjoyer2D

cmake -S . -B build
cmake --build build --target enjoyer2d_testbed --config Debug
```

Run the testbed:

```bat
build\testbed\Debug\enjoyer2d_testbed.exe
```

Build and run tests:

```bat
cmake --build build --target enjoyer2d_tests --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

If saved scenes do not appear when running from Visual Studio, set the debugger working directory to the project root folder. The project also copies `saved_scenes/` next to the executable after building the testbed.

## Testbed Controls

| Input | Action |
| --- | --- |
| `W`, `A`, `S`, `D` | Move camera |
| `I`, `O` | Zoom in/out |
| `Space` | Pause/resume simulation |
| `N` | Single-step while paused |
| `R` | Reset current built-in scene |
| Left/Right arrows | Switch built-in scene |
| Backspace | Delete selected body |
| Left mouse button | Select or spawn body, depending on active editor tool |
| Escape | Quit |

Most controls are also available through the ImGui editor panels.

## Scene Files

The testbed can save and load scenes as JSON files in `saved_scenes/`. Scene files store world settings, solver settings, and body data such as position, velocity, shape, mass, static state, friction, and restitution.

When running from an IDE, set the working directory to the project root if you want saved scenes to appear directly in the repository folder. The CMake build also copies bundled scene files next to the executable for easier running from the build directory.

## Engine Overview

The physics engine is intentionally small and readable. The main engine types are:

- `Enjoyer2D::World` - owns bodies, stores gravity, steps the simulation, detects contacts, and solves collisions.
- `Enjoyer2D::Body` - stores position, velocity, force, mass data, material data, static/dynamic state, and shape.
- `Enjoyer2D::Shape` - represents box and circle collision shapes.
- `Enjoyer2D::Contact` - stores collision normal, penetration depth, contact point, and body indices.

The testbed is a separate layer. It reads the engine state, draws debug visualization using SDL_Renderer, and sends editor commands back into the world.

## Current Scope

Enjoyer2D currently focuses on simple 2D rigid body simulation for learning and demonstration. It does not yet include rotation, angular velocity, joints, SAT polygon collision, continuous collision detection, or broad-phase spatial partitioning. These are natural future extensions.
