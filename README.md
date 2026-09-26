# SceneForge

A C++17 and OpenGL project growing into a small 3D scene editor.

SceneForge is developed one milestone at a time, with a focus on understanding the rendering pipeline, resource ownership, and application architecture. The current milestone is **M0 — Setup and first triangle**: a GLFW window rendering a triangle with a position-based color gradient.

## Current features

- OpenGL 3.3 core context created with GLFW.
- OpenGL function loading through GLAD.
- Vertex data stored in a VBO and configured through a VAO.
- Vertex and fragment shaders written in GLSL 330 core.
- Shader compilation and program linking with error logs.
- A render loop with framebuffer clearing, drawing, buffer swapping, and event handling.
- Manual OpenGL resource cleanup and a GLFW shutdown guard.
- CMake dependency fetching for GLFW and GLAD.

## Build and run

The instructions below target **Ubuntu/Debian Linux**.

### Requirements

- A C compiler and a C++ compiler supporting **C++17**.
- **CMake 3.20 or newer** and Git.
- **Python 3** with **Jinja2** for the GLAD generator.
- The X11 and Wayland development libraries used by GLFW.
- A graphical session and a graphics driver supporting **OpenGL 3.3 core**.
- Internet access to fetch dependencies during the initial CMake configuration.

Install the build tools and system dependencies:

```bash
sudo apt update
sudo apt install build-essential cmake git pkg-config python3 python3-jinja2 \
    xorg-dev libwayland-dev libxkbcommon-dev
```

CMake fetches **GLFW 3.4** and **GLAD 2.0.8** automatically. The GLAD loader is generated for OpenGL 3.3 core during the build. See the upstream [GLFW build requirements](https://www.glfw.org/docs/3.4/compile.html#compile_deps) and [GLAD generator requirements](https://github.com/Dav1dde/glad/blob/v2.0.8/requirements.txt) for details.

### Configure, build, and launch

```bash
git clone https://github.com/ven0xO/SceneForge.git
cd SceneForge

cmake -S . -B build
cmake --build build --parallel
./build/sceneforge
```

The application opens a **1280 × 720** window with a red/green gradient triangle on a black background. Close the window to exit.

Generated files and downloaded dependencies are placed under `build/`.

### Common setup issues

| Message or symptom | Check |
| --- | --- |
| CMake cannot find X11, Wayland, or xkbcommon | Install the system development packages listed above. Both GLFW backends are enabled by the current configuration. |
| Python reports that `jinja2` is missing | Install Jinja2 for the Python interpreter selected by CMake; its path appears in the configuration output. |
| The first configuration fails while fetching dependencies | Check that Git is installed and GitHub is reachable. |
| GLFW cannot create the window or OpenGL context | Run in a graphical session and check that the driver supports OpenGL 3.3 core. Read the GLFW error printed to the terminal. |

## How the triangle is rendered

The current rendering path is deliberately small enough to follow in [`src/main.cpp`](src/main.cpp):

1. GLFW creates the window and makes its OpenGL context current.
2. GLAD loads the OpenGL function pointers.
3. Three vertex positions are uploaded to a VBO. A VAO describes the three-component position attribute at location `0`.
4. The vertex and fragment shaders are compiled and linked into a program.
5. Each frame clears the framebuffer, selects the program and VAO, draws three vertices, and presents the result.

The vertex shader writes the clip-space position and passes the original position to the fragment shader. Rasterization interpolates that position across the triangle. The fragment shader calculates red from `x + 0.5`, green from `y + 0.5`, and blue from `z`.

For the current geometry, X and Y range from `-0.5` to `0.5`, so the red and green channels range from `0` to `1`. All Z coordinates are zero, so the blue channel stays at zero.

## Project structure

```text
SceneForge/
├── CMakeLists.txt    # Executable, language standard, and dependencies
├── README.md
└── src/
    └── main.cpp     # Window setup, OpenGL initialization, shaders, and rendering
```

At M0, initialization and rendering live in one file. M1 will introduce RAII wrappers as a focused exercise in ownership, resource lifetime, and move semantics.

## Roadmap

| Milestone | Focus | Status |
| --- | --- | --- |
| **M0** | GLFW, GLAD, CMake, first triangle, and basic error handling | **Current** |
| M1 | RAII wrappers for shaders, buffers, and vertex arrays; a cube | Planned |
| M2 | Transforms, camera controls, delta time, depth testing, and resizing | Planned |
| M3 | Scene objects, multiple objects, and clear resource ownership | Planned |
| M4 | Dear ImGui hierarchy and inspector panels; object editing | Planned |
| M5 | Scene saving and loading with JSON and data validation | Planned |
| M6 | Materials, textures, lighting, and shared resources | Planned |
| M7 | Mouse picking and object selection in the viewport | Planned |
| M8 | Translation, rotation, and scale gizmos | Planned |
| M9 | Model import with meshes, normals, UVs, and textures | Planned |
| M10 | One additional rendering effect, such as shadows or an outline | Planned |
| M11 | Tests, sanitizers, CI, releases, demos, and documentation | Planned |

## Learning goals

- Understand how vertex data travels through the OpenGL pipeline.
- Manage resource ownership and lifetime using C++ RAII.
- Learn copy and move semantics through GPU resource wrappers.
- Build an understanding of transforms, camera mathematics, and coordinate spaces.
- Keep architectural decisions tied to the needs of each milestone.

The goal is to be able to explain the code and its trade-offs as the editor grows.
