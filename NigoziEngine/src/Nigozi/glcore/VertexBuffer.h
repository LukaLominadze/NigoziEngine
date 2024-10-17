#pragma once

namespace Nigozi
{
	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void Delete();
	private:
		unsigned int m_rendererID;
	};
}

