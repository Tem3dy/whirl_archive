#pragma once

#include <cstdint>

#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

class VertexArray
{
public:
    VertexArray()
    {
        glGenVertexArrays(1, &m_handle);
        WHIRL_DEBUG("Creating vertex array: {}", m_handle);
        m_index = 0;
    }

    ~VertexArray()
    {
        WHIRL_DEBUG("Deleting vertex array: {}", m_handle);
        glDeleteVertexArrays(1, &m_handle);
    }

    // Delete copy/move
    VertexArray(VertexArray&&) = delete;
    VertexArray& operator=(VertexArray&&) = delete;
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

public:
    void Bind() const
    {
        glBindVertexArray(m_handle);
    }

    void Unbind() const
    {
        glBindVertexArray(0);
    }

    void Link(const VertexBuffer& source, const std::vector<VertexAttribute>& layout)
    {
        source.Bind();
        for (int i = 0; i < layout.size(); i++)
        {
            const auto& attribute = layout[i];
            if (attribute.format == VertexFormat::FLOAT)
            {
                glVertexAttribPointer(
                    m_index,
                    attribute.size,
                    VertexLayout::GetType(attribute.format),
                    GL_FALSE,
                    VertexLayout::GetStride(layout),
                    reinterpret_cast<const void*>(VertexLayout::GetOffset(layout, i))
                );
            }
            else
            {
                glVertexAttribIPointer(
                    m_index,
                    attribute.size,
                    VertexLayout::GetType(attribute.format),
                    VertexLayout::GetStride(layout),
                    reinterpret_cast<const void*>(VertexLayout::GetOffset(layout, i))
                );
            }

            if (attribute.rate == AdvanceRate::INSTANCE)
            {
                glVertexAttribDivisor(m_index, 1);
            }

            glEnableVertexAttribArray(m_index);
            m_index++;
        }

        source.Unbind();
    }
    
private:
    uint32_t m_handle;
    uint32_t m_index;
};