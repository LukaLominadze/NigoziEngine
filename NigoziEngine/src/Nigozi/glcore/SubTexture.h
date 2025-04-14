#pragma once
#include "ngpch.h"
#include "Texture.h"

namespace Nigozi
{
	class SubTexture {
	public:
		SubTexture(const std::shared_ptr<Texture>& texture, float size, uint32_t slotX = 0, uint32_t slotY = 0);

		void SetSubTexture(float size);
		void SetSubTexture(float size, uint32_t slotX, uint32_t slotY);
		void SetSlot(uint32_t slotX, uint32_t slotY);

		inline const std::shared_ptr<Texture>& GetTexture() const { return m_texture; }

		inline const glm::vec2& GetCoordMin() { return m_coordMin; }
		inline const glm::vec2& GetCoordMax() { return m_coordMax; }
	private:
		std::shared_ptr<Texture> m_texture;

		float m_size;
		
		glm::vec2 m_textureSize;
		glm::vec2 m_coordMin;
		glm::vec2 m_coordMax;
	};
}
