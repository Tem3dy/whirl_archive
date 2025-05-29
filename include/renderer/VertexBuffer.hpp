#pragma once

#include <cstdint>
#include <vector>

#include <glad/gl.h>

#include "DrawMode.hpp"
#include "Logger.hpp"

class VertexBuffer
{
public:
    VertexBuffer()
    {
        glGenBuffers(1, &m_handle);
        WHIRL_DEBUG("Creating vertex buffer: {}", m_handle);
    }

    ~VertexBuffer()
    {
        if (m_handle != 0)
        {
            // Paranoia
            WHIRL_DEBUG("Deleting vertex buffer: {}", m_handle);
            glDeleteBuffers(1, &m_handle);
        }
    }

    // Delete copy/move
    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

public:
    template <typename T>
    void Upload(const std::vector<T>& data, DrawMode mode)
    {
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GetMode(mode));
    }

    void Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    }

    void Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

private:
    uint32_t m_handle = 0;
};