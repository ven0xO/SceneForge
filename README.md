# SceneForge

A C++17 and OpenGL project growing into a small 3D scene editor.

SceneForge is developed one milestone at a time, with a focus on understanding the rendering pipeline, resource ownership, and application architecture. The current milestone is **M1 — Renderer foundations and RAII**, in progress. The demo renders a cube with a position-based RGB gradient.

## Current features

- OpenGL 3.3 core context created with GLFW.
- OpenGL function loading through GLAD.
- A cube made of 36 non-indexed vertices: two triangles per face.
- Vertex data stored in a VBO and configured through a VAO.
- Vertex and fragment shaders loaded from GLSL 330 core source files.
- Shader compilation and program linking with error logs.
- RAII ownership for shader programs, vertex buffers, and vertex arrays, with copying disabled.
- Fixed rotations around X and Y, depth testing, and a centered square viewport sized to the framebuffer.
- A render loop with color and depth clearing, drawing, buffer swapping, and event handling.
- A GLFW shutdown guard.
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

The application opens a **1280 × 720** window with a statically rotated RGB gradient cube on a black background. Close the window to exit. Run from the repository root so the relative paths to the shader files resolve correctly.

Generated files and downloaded dependencies are placed under `build/`.

### Common setup issues

| Message or symptom | Check |
| --- | --- |
| CMake cannot find X11, Wayland, or xkbcommon | Install the system development packages listed above. Both GLFW backends are enabled by the current configuration. |
| Python reports that `jinja2` is missing | Install Jinja2 for the Python interpreter selected by CMake; its path appears in the configuration output. |
| The first configuration fails while fetching dependencies | Check that Git is installed and GitHub is reachable. |
| GLFW cannot create the window or OpenGL context | Run in a graphical session and check that the driver supports OpenGL 3.3 core. Read the GLFW error printed to the terminal. |

## How the cube is rendered

The rendering setup and loop live in [`src/main.cpp`](src/main.cpp), with resource ownership handled by the `VertexBuffer`, `VertexArray`, and `Shader` wrappers:

1. GLFW creates the window and makes its OpenGL context current.
2. GLAD loads the OpenGL function pointers.
3. The cube's 36 vertex positions are uploaded to a VBO. A VAO describes the three-component position attribute at location `0`.
4. `Shader` loads the vertex and fragment source files, compiles them, and links the program.
5. Each frame fits a centered square viewport to the framebuffer, clears color and depth, selects the program and VAO, draws 36 vertices, and presents the result.
6. The resource wrappers are destroyed before the window and its OpenGL context.

The vertex shader applies fixed rotations around X and Y to produce clip-space coordinates. It also passes the original local position to the fragment shader. Rasterization interpolates this position, and the fragment shader maps it to RGB with `vertexPosition + 0.5`. Each local coordinate ranges from `-0.5` to `0.5`, giving color values from `0` to `1`.

Fixed rotation and depth testing provide a simple cube demo before the camera and transform system planned for M2. The shader files retain their original names, `shaders/triangle.vert` and `shaders/triangle.frag`.

## Project structure

```text
SceneForge/
├── CMakeLists.txt       # Executable, language standard, and dependencies
├── README.md
├── shaders/
│   ├── triangle.vert   # Fixed cube rotation and local position output
│   └── triangle.frag   # Position-based RGB gradient
└── src/
    ├── main.cpp        # Window, initialization, cube geometry, and render loop
    ├── constants.hpp   # Window dimensions and development settings
    ├── Shader.hpp      # Shader program ownership and interface
    ├── Shader.cpp
    ├── VertexBuffer.hpp
    ├── VertexBuffer.cpp
    ├── VertexArray.hpp
    └── VertexArray.cpp
```

The first triangle completed M0. M1 now includes a cube and non-copyable RAII wrappers for the shader program, VBO, and VAO. **IndexBuffer/EBO support and move semantics remain to complete M1.**

## Roadmap

| Milestone | Focus | Status |
| --- | --- | --- |
| M0 | GLFW, GLAD, CMake, first triangle, and basic error handling | Complete |
| **M1** | RAII wrappers, IndexBuffer/EBO, move semantics, and a cube | **Current** |
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
