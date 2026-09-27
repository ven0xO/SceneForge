#include <algorithm>
#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "constants.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"


// Terminate GLFW when main leaves scope, including early returns.
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

// Report GLFW errors from initialization and window operations.
void onGlfwError(int code, const char* description)
{
    std::cerr << "GLFW [" << code << "]: " << description << "\n";
}

int main()
{
        // Register diagnostics and initialize GLFW under automatic cleanup.
        glfwSetErrorCallback(onGlfwError);
        GlfwGuard glfwGuard;
        if(!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW\n";
            return -1;
        }
        else if(DEV)
        {
            std::cout << "SUCCESS - Initialized GLFW\n";
        }

        // Request an OpenGL 3.3 core context.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create the window and its OpenGL context.
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SceneForge", nullptr, nullptr);

    // End the GPU resource scope before destroying the window and its context.
    {
        // Six faces, two triangles per face, three positions per triangle.
        constexpr float cube_vert[] = {
            // Positive Z face.
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            // Negative Z face.
             0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,

            // Negative X face.
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            // Positive X face.
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,

            // Positive Y face.
            -0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,

            // Negative Y face.
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
        };
        constexpr GLsizei cube_vertex_count = sizeof(cube_vert) / (3 * sizeof(cube_vert[0]));

        if (window == nullptr)
        {
            std::cerr << "Failed to create GLFW window.\n";
            return -1;
        }
        else if(DEV)
        {
            std::cout << "SUCCESS - Created GLFW window\n";
        }

        // Select the context before loading its OpenGL functions.
        glfwMakeContextCurrent(window);

        // Load OpenGL function pointers through GLAD.
        if(gladLoadGL(glfwGetProcAddress) == 0)
        {
            std::cerr << "Failed to get GL adress\n";
            return -1;
        }
        else if(DEV)
        {
            std::cout << "SUCCESS - Got GL adress\n";
        }

        // Synchronize buffer swaps with the display refresh.
        glfwSwapInterval(1);

        // Upload the cube positions; shared corners are repeated for each triangle.
        VertexBuffer vbo(cube_vert, sizeof(cube_vert));
        // Configure position attribute 0 with a stride of three floats.
        VertexArray vao(vbo, 3, GL_FLOAT, 3*sizeof(cube_vert[0]));

        // Load the GLSL files and link the program used for rendering.
        Shader shader("shaders/triangle.vert", "shaders/triangle.frag");

        // Let nearer faces hide farther faces, regardless of draw order.
        glEnable(GL_DEPTH_TEST);

        // Set the background color and render until the window is closed.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        while (!glfwWindowShouldClose(window))
        {
            // Keep a centered square viewport so the cube is not stretched.
            int framebufferWidth = 0;
            int framebufferHeight = 0;
            glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
            const int viewportSize = std::min(framebufferWidth, framebufferHeight);
            glViewport((framebufferWidth - viewportSize) / 2,
                       (framebufferHeight - viewportSize) / 2,
                       viewportSize, viewportSize);

            // Clear both color and depth before drawing the next frame.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Draw all twelve triangles using the shader and VAO wrappers.
            shader.use();
            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, cube_vertex_count);
            // Present the frame and process window events.
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // Destroy the shader, VAO, and VBO here while the context is current.
    }
    
    // Destroy the window after the GPU resource wrappers have left scope.
    glfwDestroyWindow(window);

}
