#pragma once

#include "glad/gl.h"

class VertexBuffer
{
public:
    VertexBuffer(const float* vert, GLsizeiptr BytesSize);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;
    
    void bind() const;

private:
    unsigned int VBO{0};
};
