#include "ngpch.h"

#include "VertexBuffer.h"

namespace Nigozi
{
    VertexBuffer::VertexBuffer(const void* data, uint32_t size)
    {
        GLCall(glGenBuffers(1, &m_rendererID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }

    VertexBuffer::~VertexBuffer()
    {
        Delete();
    }

    void VertexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    }

    void VertexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }

    void VertexBuffer::SetData(const void* data, uint32_t size)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }

    void VertexBuffer::Delete()
    {
        if (m_rendererID == -1) {
            GLCall(glDeleteBuffers(1, &m_rendererID));
            m_rendererID = -1;
        }
    }
}
