#pragma once

namespace Nigozi
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const std::string& filePath);
		Texture(Texture&& other) noexcept;
		~Texture();

		void Bind(uint32_t slot) const;
		void Unbind() const;

		void Delete();

		inline int GetWidth() const { return m_size.x; }
		inline int GetHeight() const { return m_size.y; }
		inline glm::i32vec2 GetSize() { return m_size; }

		inline const std::string& GetPath() const { return m_filePath; }

		inline const uint32_t GetRendererID() const { return m_rendererID; }
	private:
		uint32_t m_rendererID;
		std::string m_filePath;
		unsigned char* m_localBuffer;
		glm::i32vec2 m_size;
		int m_BPP;
	};
}

