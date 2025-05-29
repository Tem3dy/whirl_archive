#include <glad/gl.h>

#include "VertexBuffer.hpp"
#include "Logger.hpp"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_handle);
    WHIRL_DEBUG("Creating vertex buffer: {}", m_handle);
}

VertexBuffer::~VertexBuffer()
{
    if (m_handle != 0)
    {
        // Paranoia
        WHIRL_DEBUG("Deleting vertex buffer: {}", m_handle);
        glDeleteBuffers(1, &m_handle);
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_handle(other.m_handle)
{
    other.m_handle = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        m_handle = other.m_handle;
        other.m_handle = 0;
    }

    return *this;
}

void VertexBuffer::Data(const void* data, uint32_t size, DrawMode mode)
{
    glBufferData(GL_ARRAY_BUFFER, size, data, GetMode(mode));
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

uint32_t VertexBuffer::GetBuffer() const
{
    return m_handle;
}