#pragma once

#include "glad/gl.h"
#include "VertexBuffer.hpp"

// Owns vertex input state; the supplied VertexBuffer keeps ownership of its buffer.
class VertexArray
{
public:
    // Configure attribute 0; bytesSize is the stride between vertices in bytes.
    VertexArray(const VertexBuffer& vertexBuffer, GLint arraySize, GLenum arrayType, GLsizeiptr bytesSize);
    ~VertexArray();

    // Prevent multiple objects from owning and deleting the same VAO.
    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;
    
    // Select this VAO's stored vertex input configuration.
    void bind() const;

private:
    unsigned int VAO{0};
};
