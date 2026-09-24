cmake_minimum_required(VERSION 3.20)

project(SceneForge LANGUAGES CXX)

add_executable(sceneforge src/main.cpp)
target_compile_features(sceneforge PRIVATE cxx_std_17)

include(FetchContent)
FetchContent_Declare(
    glfw 
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)
target_link_libraries(sceneforge PRIVATE glfw)