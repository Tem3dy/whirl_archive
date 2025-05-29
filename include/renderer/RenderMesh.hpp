#pragma once

#include <cstdint>
#include <cstddef>
#include <memory>

#include "VertexLayout.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

enum class RenderMode
{
    COMMON, INSTANCED,
};

class RenderMesh
{
public:
    RenderMesh(RenderMode mode = RenderMode::COMMON) : 
        m_mode(mode)
    {
        WHIRL_DEBUG("Creating a render mesh");
    }

    ~RenderMesh()
    {
        WHIRL_DEBUG("Deleting a render mesh");
    }

public:
    RenderMesh(const RenderMesh&) = delete;
    RenderMesh(RenderMesh&&) = delete;

    RenderMesh& operator=(const RenderMesh&) = delete;
    RenderMesh& operator=(RenderMesh&&) = delete;

public:
    void Bind() const
    {
        m_layout.Bind();
    }

    void Unbind() const
    {
        m_layout.Unbind();
    }

    void SetMode(RenderMode mode)
    {
        m_mode = mode;
    }

    void Configure() const
    {
        // The only reason this exists is so that the VAO of the mesh remembers the IBO
        m_layout.Bind();
        m_indices.Bind();
        m_layout.Unbind();
        m_indices.Unbind();
    }

    void Draw(uint32_t count = 0) const
    {
        // Make sure the VAO is bound
        if (m_mode == RenderMode::COMMON)
        {
            glDrawElements(GL_TRIANGLES, m_indices.GetCount(), GL_UNSIGNED_INT, nullptr);
        }
        else
        {
            if (count > 0)
            {
                glDrawElementsInstanced(GL_TRIANGLES, m_indices.GetCount(), GL_UNSIGNED_INT, nullptr, count);
            }
            else
            {
                WHIRL_WARN("Tried to draw 0 instances");
            }
        }
    }

    template <typename T>
    void UploadVertices(const std::vector<T>& data, DrawMode mode)
    {
        m_vertices.Bind();
        m_vertices.Upload(data, mode);
        m_vertices.Unbind();
    }

    template <typename T>
    void UploadInstances(const std::vector<T>& data, DrawMode mode)
    {
        m_instances.Bind();
        m_instances.Upload(data, mode);
        m_instances.Unbind();
    }

    template <typename T>
    void UploadIndices(const std::vector<T>& data, DrawMode mode)
    {
        m_indices.Bind();
        m_indices.Upload(data, mode);
        m_indices.Unbind();
    }

    void LinkVertexLayout(const std::vector<VertexAttribute>& layout)
    {
        m_layout.Bind();
        m_layout.Link(m_vertices, layout);
        m_layout.Unbind();
    }

    void LinkInstanceLayout(const std::vector<VertexAttribute>& layout)
    {
        m_layout.Bind();
        m_layout.Link(m_instances, layout);
        m_layout.Unbind();
    }

private:
    RenderMode m_mode;
    VertexArray m_layout;
    VertexBuffer m_vertices;
    IndexBuffer m_indices;
    VertexBuffer m_instances;
};