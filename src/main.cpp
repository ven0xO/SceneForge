#include <iostream>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "constants.hpp"


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
    // Store three tightly packed 3D vertex positions on the CPU.
    constexpr float triangle_vert[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Use vertex positions as clip coordinates and forward them for coloring.
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;

        out vec3 vertexPosition;

        void main()
        {
            vertexPosition = aPos;
            gl_Position = vec4(aPos, 1.0f);
        }
    )";

    // Build a color gradient from interpolated vertex positions.
    const char* fragmentShaderSource = R"(
        #version 330 core

        in vec3 vertexPosition;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(vertexPosition.x + 0.5, vertexPosition.y + 0.5, vertexPosition.z, 1.0f);
        }
    )";

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

    // Create the vertex array and buffer used by the triangle.
    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // Upload the vertex data to the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vert), triangle_vert, GL_STATIC_DRAW);

    // Record three-float positions and their VBO in VAO attribute 0.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Create the vertex shader object.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if(vertexShader == 0)
    {
        std::cerr << "Failed to create vertex shader\n";
        return -1;
    } else if(DEV)
    {
        std::cout << "SUCCESS - Created vertex shader\n";
    }

    // Compile the embedded vertex shader source.
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check compilation and report the driver's diagnostic log on failure.
    GLint status{GL_FALSE};
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE)
    {
        char log[1024]{};
        glGetShaderInfoLog(vertexShader, 1024, nullptr, log);
        std::cerr << log << std::endl;
        glDeleteShader(vertexShader);
        return -1;
    } else if(DEV)
    {
        std::cout << "SUCCESS - Compiled vertex shader\n";
    }

    // Create the fragment shader object.
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if(fragmentShader == 0)
    {
        std::cerr << "Failed to create fragment shader\n";
        glDeleteShader(vertexShader);
        return -1;
    } else if(DEV)
    {
        std::cout << "SUCCESS - Created fragment shader\n";
    }


    // Compile the embedded fragment shader source.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check fragment shader compilation and release shaders on failure.
    status = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE)
    {
        char log[1024]{};
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, log);
        std::cerr << log << std::endl;
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        return -1;
    } else if(DEV)
    {
        std::cout << "SUCCESS - Compiled fragment shader\n";
    }

    // Create a program to combine the compiled shader stages.
    GLuint shaderProgram = glCreateProgram();

    if(shaderProgram == 0)
    {
        std::cerr << "Failed to create program\n";
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return -1;
    }else if(DEV)
    {
        std::cout << "SUCCESS - Created program\n";
    }

    // Link both shader stages into an executable program.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking and release the program and shaders on failure.
    status = GL_FALSE;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

    if(status != GL_TRUE)
    {
        char log[1024]{};
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, log);
        std::cerr << log << std::endl;
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return -1;
    }

    // Set the background color and render until the window is closed.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        // Clear the back buffer before drawing a new frame.
        glClear(GL_COLOR_BUFFER_BIT);
        // Select the program and vertex layout, then draw three vertices.
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Present the frame and process window events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Release OpenGL resources while the context is still available.
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    // Destroy the window and its context before the guard terminates GLFW.
    glfwDestroyWindow(window);

}
