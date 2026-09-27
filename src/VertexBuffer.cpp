#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const float* vert, GLsizeiptr bytesSize)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bytesSize, vert, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &VBO);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}