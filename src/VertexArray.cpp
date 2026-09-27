#include "VertexArray.hpp"

VertexArray::VertexArray(const VertexBuffer& vertexBuffer, GLint arraySize, GLenum arrayType, GLsizeiptr bytesSize)
{
    // Create and select the VAO that will store the attribute configuration.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Store the attribute layout and its VBO association in this VAO.
    // arraySize is the component count; bytesSize is the vertex stride in bytes.
    vertexBuffer.bind();
    glVertexAttribPointer(0, arraySize, arrayType, GL_FALSE, bytesSize, nullptr);
    glEnableVertexAttribArray(0);

}

VertexArray::~VertexArray()
{
    // Release this VAO while a context is current; the VBO has its own owner.
    glDeleteVertexArrays(1, &VAO);
}

void VertexArray::bind() const
{
    // Restore this VAO's stored vertex input state for drawing or configuration.
    glBindVertexArray(VAO);
}