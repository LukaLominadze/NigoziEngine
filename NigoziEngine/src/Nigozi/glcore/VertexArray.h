#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "ref/Ref.h"

namespace Nigozi
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(VertexArray&& other) noexcept;
		~VertexArray();

		void AddVertexBuffer(const Ref<VertexBuffer>& vbo, const VertexBufferLayout& vboLayout);
		void AddIndexBuffer(const Ref<IndexBuffer>& ibo);

		void Bind() const;
		void Unbind() const;

		void SetVertexBufferData(const void* data, uint32_t size);
		void SetIndexBufferData(const void* data, uint32_t count);

		void Delete();
	private:
		uint32_t m_rendererID;

		Ref<VertexBuffer> r_vbo;
		Ref<IndexBuffer> r_ibo;
	};
}

