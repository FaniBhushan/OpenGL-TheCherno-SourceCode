#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    // creates/generate 1 buffer object name (since the first parameter is 1)
    GLCALL(glGenBuffers(1, &m_RendererID));

    // bind the buffer object name against the target "GL_ARRAY_BUFFER"
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

    // creates and initializes the buffer data store
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const
{
    // bind the buffer object name against the target "GL_ARRAY_BUFFER"
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}