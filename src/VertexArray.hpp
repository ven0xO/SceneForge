#pragma once

#include "glad/gl.h"
#include "VertexBuffer.hpp"

class VertexArray
{
public:
    VertexArray(const VertexBuffer& vertexBuffer, GLint arraySize, GLenum arrayType, GLsizeiptr bytesSize);
    ~VertexArray();

    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;
    
    void bind() const;

private:
    unsigned int VAO{0};
};
