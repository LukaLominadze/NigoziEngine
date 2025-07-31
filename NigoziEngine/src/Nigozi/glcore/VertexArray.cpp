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

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo, const VertexBufferLayout& vboLayout)
	{
		r_vbo = vbo;
		r_vbo->Bind();
		const std::vector<VertexBufferLayoutElement>& elements = vboLayout.GetElements();
		uint32_t offset = 0;
		for (size_t i = 0; i < elements.size(); i++) {
			const VertexBufferLayoutElement& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			GLCall(glVertexAttribPointer(i, element.Count, element.Type, element.Normalized,
				vboLayout.GetStride(), (const void*)offset));
			offset += element.Count * VertexBufferLayoutElement::GetTypeSize(element.Type);
		}
	}

	void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo)
	{
		r_ibo = ibo;
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_rendererID));
		r_ibo->Bind();
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
		r_ibo->Unbind();
	}

	void VertexArray::SetVertexBufferData(const void* data, uint32_t size)
	{
		r_vbo.get()->Bind();
		r_vbo.get()->SetData(data, size);
	}

	void VertexArray::SetIndexBufferData(const void* data, uint32_t count)
	{
		r_ibo.get()->Bind();
		r_ibo.get()->SetData(data, count);
	}

	void VertexArray::Delete()
	{
		if (m_rendererID == -1) {
			GLCall(glDeleteVertexArrays(1, &m_rendererID));
			m_rendererID = -1;
		}
	}
}
