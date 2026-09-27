#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer(const float* vert, GLsizeiptr bytesSize)
{
    // Create a vertex buffer and copy the supplied data into OpenGL-managed storage.
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bytesSize, vert, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer()
{
    // Release the buffer owned by this object.
    glDeleteBuffers(1, &VBO);
}

void VertexBuffer::bind() const
{
    // Select this buffer as the current GL_ARRAY_BUFFER.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}