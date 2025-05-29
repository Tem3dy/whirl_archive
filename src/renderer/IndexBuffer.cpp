#include <glad/gl.h>

#include "IndexBuffer.hpp"
#include "Logger.hpp"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_handle);
    WHIRL_DEBUG("Creating index buffer: {}", m_handle);
}

IndexBuffer::~IndexBuffer()
{
    if (m_handle != 0)
    {
        // Paranoia
        WHIRL_DEBUG("Deleting index buffer: {}", m_handle);
        glDeleteBuffers(1, &m_handle);
    }
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_handle(other.m_handle)
{
    other.m_handle = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        m_handle = other.m_handle;
        other.m_handle = 0;
    }

    return *this;
}

void IndexBuffer::Data(const void* data, uint32_t size, DrawMode mode)
{
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GetMode(mode));
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t IndexBuffer::GetBuffer() const
{
    return m_handle;
}