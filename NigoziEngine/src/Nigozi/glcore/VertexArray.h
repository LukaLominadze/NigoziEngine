#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Nigozi
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& vboLayout);

		void Bind() const;
		void Unbind() const;

		void Delete();
	private:
		unsigned int m_rendererID;
	};
}

