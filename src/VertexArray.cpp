#include "VertexArray.hpp"

VertexArray::VertexArray(const VertexBuffer& vertexBuffer, GLint arraySize, GLenum arrayType, GLsizeiptr bytesSize)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    vertexBuffer.bind();
    glVertexAttribPointer(0, arraySize, arrayType, GL_FALSE, bytesSize, nullptr);
    glEnableVertexAttribArray(0);

}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &VAO);
}

void VertexArray::bind() const
{
    glBindVertexArray(VAO);
}