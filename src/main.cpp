#include <iostream>
#include "GLFW/glfw3.h"

constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;
constexpr bool DEV = true;

class GlfwGuard
{
public:
    GlfwGuard() = default;

    ~GlfwGuard()
    {
        glfwTerminate();
        if(DEV)
        {
            std::cout << "SUCCESS - Terminated GLFW\n";
        }
    }
};

void onGlfwError(int code, const char* description)
{
    std::cerr << "GLFW [" << code << "]: " << description << "\n";
}

int main()
{
    glfwSetErrorCallback(onGlfwError);
    GlfwGuard glfwGuard;
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    else if(DEV)
    {
        std::cout << "SUCCESS - Initialize GLFW\n";
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SceneForge", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window.\n";
        return -1;
    }
    else if(DEV)
    {
        std::cout << "SUCCESS - Created GLFW window\n";
    }

    glfwMakeContextCurrent(window);

    

}