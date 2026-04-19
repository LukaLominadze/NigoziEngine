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
	public:
		VertexBuffer& operator=(VertexBuffer&& other) noexcept
		{
			m_rendererID = other.m_rendererID;

			// Invalidate
			other.m_rendererID = -1;
			return *this;
		}
	private:
		uint32_t m_rendererID;
	};
}

