#include "ngpch.h"

#include "IndexBuffer.h"

namespace Nigozi
{

    IndexBuffer::IndexBuffer(const void* data, unsigned int count)
        :m_count(count)
    {
        ASSERT(sizeof(unsigned int) == sizeof(GLuint), "Checking if GLuint int is 4 bytes...");

        GLCall(glGenBuffers(1, &m_rendererID));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
    }

    IndexBuffer::~IndexBuffer()
    {
        Delete();
    }

    void IndexBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
    }

    void IndexBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    void IndexBuffer::SetData(const void* data, unsigned int count)
    {
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
    }

    void IndexBuffer::Delete()
    {
        if (m_rendererID == -1) {
            GLCall(glDeleteBuffers(1, &m_rendererID));
            m_rendererID = -1;
        }
    }
}
