cmake_minimum_required(VERSION 3.20)

project(SceneForge LANGUAGES C CXX)

add_executable(sceneforge src/main.cpp)
target_compile_features(sceneforge PRIVATE cxx_std_17)

include(FetchContent)
FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.4
)

FetchContent_Declare(
    glad 
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git 
    GIT_TAG v2.0.8 
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glad)
target_link_libraries(sceneforge PRIVATE glfw)
target_link_libraries(sceneforge PRIVATE glad_opengl)

glad_add_library(glad_opengl STATIC REPRODUCIBLE API gl:core=3.3)
