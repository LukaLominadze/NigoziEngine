#pragma once

namespace Nigozi
{
	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(const void* data, uint32_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);

		void Delete();
	private:
		uint32_t m_rendererID;
	};
}

