#pragma once

#include <cstdint>
#include <vector>

#include <glad/gl.h>

#include <DrawMode.hpp>
#include <Logger.hpp>

class IndexBuffer
{
public:
    IndexBuffer()
    {
        glGenBuffers(1, &m_handle);
        WHIRL_DEBUG("Creating index buffer: {}", m_handle);
    }

    ~IndexBuffer()
    {
        if (m_handle != 0)
        {
            // Paranoia
            WHIRL_DEBUG("Deleting index buffer: {}", m_handle);
            glDeleteBuffers(1, &m_handle);
        }
    }

    // Delete copy/move
    IndexBuffer(IndexBuffer&& other) = delete;
    IndexBuffer& operator=(IndexBuffer&& other) = delete;
    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

public:
    template <typename T>
    void Upload(const std::vector<T>& data, DrawMode mode)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GetMode(mode));
        m_count = data.size();
    }

    void Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    }

    void Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t GetCount() const
    {
        return m_count;
    }

private:
    uint32_t m_handle = 0;
    uint32_t m_count = 0;
};