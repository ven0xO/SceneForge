# Configure the C/C++ project; GLAD generates C sources.
cmake_minimum_required(VERSION 3.20)

project(SceneForge LANGUAGES C CXX)

# Build the application from its source files.
add_executable(sceneforge 
    src/main.cpp
    src/VertexBuffer.cpp
    src/VertexArray.cpp
    src/Shader.cpp
    )
# Require C++17 features for the application.
target_compile_features(sceneforge PRIVATE cxx_std_17)

# Enable CMake's dependency fetching support.
include(FetchContent)
# Fetch GLFW for window creation, context management, and input.
FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.4
)

# Fetch GLAD's OpenGL loader generator and CMake helpers.
FetchContent_Declare(
    glad 
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git 
    GIT_TAG v2.0.8 
    SOURCE_SUBDIR cmake
)

# Download and configure the declared dependencies.
FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glad)
# Link the application to GLFW and the generated OpenGL loader.
target_link_libraries(sceneforge PRIVATE glfw)
target_link_libraries(sceneforge PRIVATE glad_opengl)

# Generate a static GLAD loader for the OpenGL 3.3 core API.
glad_add_library(glad_opengl STATIC REPRODUCIBLE API gl:core=3.3)
