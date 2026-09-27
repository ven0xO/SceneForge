#pragma once

#include "string"
#include "glad/gl.h"
#include "iostream"
#include <stdexcept>
#include <fstream>

#include "constants.hpp"

// Owns one linked OpenGL program and releases it through RAII.
// Create, use, and destroy it while a compatible OpenGL context is current.
class Shader
{
public:

    // Load GLSL files, compile both shaders, and link the program.
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    // Prevent two objects from owning and deleting the same program.
    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    // Select this program for subsequent rendering commands.
    void use() const;

private:
    // Read shader source text and report file errors with exceptions.
    const std::string readFile(const std::string& filePath);

    // The OpenGL program handle owned by this object.
    GLuint shaderProgram = 0;
};
