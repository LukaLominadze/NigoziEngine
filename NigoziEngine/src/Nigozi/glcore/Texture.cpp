#include "ngpch.h"

#include "Texture.h"
#include "Nigozi/core/Assert.h"

namespace Nigozi
{
	Texture::Texture(const std::string& filePath)
		:m_rendererID(0), m_filePath(filePath), m_localBuffer(nullptr), m_width(0), m_height(0), m_BPP(0)
	{
		// Load image
		stbi_set_flip_vertically_on_load(1);
		m_localBuffer = stbi_load(filePath.c_str(), &m_width, &m_height, &m_BPP, 4);

		GLCall(glGenTextures(1, &m_rendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		
		if (m_localBuffer) {
			stbi_image_free(m_localBuffer);
		}
	}

	Texture::~Texture()
	{
		Delete();
	}

	void Texture::Bind(uint32_t slot = 0) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererID));
	}

	void Texture::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::Delete()
	{
		if (m_rendererID != -1) {
			GLCall(glDeleteTextures(1, &m_rendererID));
			m_rendererID = -1;
		}
	}
}
