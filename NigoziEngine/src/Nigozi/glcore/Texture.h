#pragma once

#include "DebugNMacros.h"

namespace Nigozi
{
	class Texture
	{
	public:
		Texture() = default;
		Texture(const std::string& filePath);
		~Texture();

		void Bind(unsigned int slot) const;
		void Unbind() const;

		void Delete();

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
	private:
		unsigned int m_rendererID;
		std::string m_filePath;
		unsigned char* m_localBuffer;
		int m_width, m_height, m_BPP;
	};
}

