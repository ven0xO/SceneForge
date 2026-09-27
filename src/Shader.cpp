#include "Shader.hpp"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    // Read both files before allocating OpenGL resources.
    // Keep the strings alive while glShaderSource copies their text.
    std::string sourceVertex = readFile(vertexPath);
    const char* vertexShaderSource = sourceVertex.c_str();

    std::string sourceFragment = readFile(fragmentPath);
    const char* fragmentShaderSource = sourceFragment.c_str();


    // Create the vertex shader and check whether allocation succeeded.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if(vertexShader == 0)
    {
        throw std::runtime_error("Failed to create vertex shader");
    } else if(DEV)
    {
        std::cout << "SUCCESS - Created vertex shader\n";
    }

    // Copy the vertex source into OpenGL and compile it.
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check compilation; on failure, collect the log and release the shader.
    GLint status{GL_FALSE};
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE)
    {
        char log[1024]{};
        glGetShaderInfoLog(vertexShader, 1024, nullptr, log);
        glDeleteShader(vertexShader);
        throw std::runtime_error(log);
    } else if(DEV)
    {
        std::cout << "SUCCESS - Compiled vertex shader\n";
    }
    

    // Create the fragment shader; release the vertex shader if this fails.
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if(fragmentShader == 0)
    {
        glDeleteShader(vertexShader);
        throw std::runtime_error("Failed to create fragment shader");
    } else if(DEV)
    {
        std::cout << "SUCCESS - Created fragment shader\n";
    }

    // Copy the fragment source into OpenGL and compile it.
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check compilation; on failure, release both shaders and report the log.
    status = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE)
    {
        char log[1024]{};
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, log);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
        throw std::runtime_error(log);
    } else if(DEV)
    {
        std::cout << "SUCCESS - Compiled fragment shader\n";
    }

    // Create the program; release both shaders if allocation fails.
    shaderProgram = glCreateProgram();

    if(shaderProgram == 0)
    {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw std::runtime_error("Failed to create program");
    }else if(DEV)
    {
        std::cout << "SUCCESS - Created program\n";
    }

    // Attach the compiled stages and link them into a rendering program.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check linking; on failure, release all resources and report the log.
    status = GL_FALSE;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

    if(status != GL_TRUE)
    {
        char log[1024]{};
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, log);
        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw std::runtime_error(log);
    }

    // Mark the attached shaders for deletion; the linked program stays usable.
    // Actual deletion waits until they are detached from all programs.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Select the program without changing the handle stored in this object.
void Shader::use() const
{
    glUseProgram(shaderProgram);
}

// Load a text file into a single string for glShaderSource.
const std::string Shader::readFile(const std::string& filePath)
{
    // The stream closes the file automatically on return or exception (RAII).
    std::ifstream file(filePath);

    // Report a missing file or another failure to open it.
    if(!file)
    {
        throw std::runtime_error("Cannot open file: " + filePath);
    }

    std::string source;
    std::string line;

    // Restore the newlines removed by getline to preserve GLSL line boundaries.
    while(std::getline(file, line))
    {
        source += line;
        source += '\n';
    }

    // Report I/O errors; reaching the end of the file is expected.
    if (file.bad())
    {
        throw std::runtime_error("Error reading file: " + filePath);
    }

    return source;
}

// Release the owned program before its OpenGL context is destroyed.
Shader::~Shader()
{
    glDeleteProgram(shaderProgram);
}