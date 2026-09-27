#pragma once

#include "glad/gl.h"

// Owns one OpenGL buffer; creation and destruction require a current context.
class VertexBuffer
{
public:
    VertexBuffer(const float* vert, GLsizeiptr bytesSize);
    ~VertexBuffer();

    // Prevent multiple objects from owning and deleting the same buffer.
    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    
    // Select this buffer for subsequent GL_ARRAY_BUFFER operations.
    void bind() const;

private:
    unsigned int VBO{0};
};
