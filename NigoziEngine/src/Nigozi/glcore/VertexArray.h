#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

namespace Nigozi
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo, const VertexBufferLayout& vboLayout);
		void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo);

		void Bind() const;
		void Unbind() const;

		void SetVertexBufferData(const void* data, uint32_t size);
		void SetIndexBufferData(const void* data, uint32_t count);

		void Delete();
	private:
		uint32_t m_rendererID;

		std::shared_ptr<VertexBuffer> r_vbo;
		std::shared_ptr<IndexBuffer> r_ibo;
	};
}

