#include <iostream>
#include "glad/gl.h"
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
    constexpr float triangle_vert[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

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

    const char* fragmentShaderSource = R"(
        #version 330 core

        in vec3 vertexPosition;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(vertexPosition.x + 0.5, vertexPosition.y + 0.5, vertexPosition.z, 1.0f);
        }
    )";

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

    if(gladLoadGL(glfwGetProcAddress) == 0)
    {
        std::cerr << "Failed to get GL adress\n";
        return -1;
    }
    else if(DEV)
    {
        std::cout << "SUCCESS - Got GL adress\n";
    }

    glfwSwapInterval(1);

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vert), triangle_vert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if(vertexShader == 0)
    {
        std::cerr << "Failed to create vertex shader\n";
        return -1;
    } else if(DEV)
    {
        std::cout << "SUCCESS - Created vertex shader\n";
    }

    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

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


    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

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

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);

}
