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
	private:
		uint32_t m_rendererID;
		uint32_t m_count;
	};
}

