#include "ngpch.h"

#include "VertexArray.h"

namespace Nigozi
{
	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_rendererID));
		GLCall(glBindVertexArray(m_rendererID));
	}

	VertexArray::~VertexArray()
	{
		Delete();
	}

	void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& vboLayout)
	{
		vbo.Bind();
		const std::vector<VertexBufferLayoutElement>& elements = vboLayout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const VertexBufferLayoutElement& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.Count, element.Type, element.Normalized,
				vboLayout.GetStride(), (const void*)offset));
			offset += element.Count * VertexBufferLayoutElement::GetTypeSize(element.Type);
		}
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_rendererID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::Delete()
	{
		if (m_rendererID == -1) {
			GLCall(glDeleteVertexArrays(1, &m_rendererID));
			m_rendererID = -1;
		}
	}
}
