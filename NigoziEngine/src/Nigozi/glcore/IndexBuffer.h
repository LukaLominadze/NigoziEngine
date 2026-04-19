#pragma once

namespace Nigozi
{
	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(const void* data, uint32_t count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t count);

		inline uint32_t GetCount() const { return m_count; }

		void Delete();
	public:
		IndexBuffer& operator=(IndexBuffer&& other) noexcept
		{
			m_rendererID = other.m_rendererID;
			m_count = other.m_count;

			// Invalidate
			other.m_rendererID = -1;
			return *this;
		}
	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};
}

