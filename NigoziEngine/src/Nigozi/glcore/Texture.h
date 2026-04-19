#pragma once

namespace Nigozi
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const std::string& filePath);
		~Texture();

		void Bind(uint32_t slot) const;
		void Unbind() const;

		void Delete();

		inline int GetWidth() const { return m_size.x; }
		inline int GetHeight() const { return m_size.y; }
		inline glm::i32vec2 GetSize() { return m_size; }

		inline const std::string& GetPath() const { return m_filePath; }

		inline const uint32_t GetRendererID() const { return m_rendererID; }
	public:
		Texture& operator=(Texture&& other) noexcept 
		{
			m_rendererID = other.m_rendererID;
			m_filePath = std::move(other.m_filePath);
			m_size = other.m_size;
			m_BPP = other.m_BPP;

			// Invalidate
			other.m_rendererID = -1;
			return *this;
		}
	private:
		uint32_t m_rendererID;
		std::string m_filePath;
		unsigned char* m_localBuffer;
		glm::i32vec2 m_size;
		int m_BPP;
	};
}

