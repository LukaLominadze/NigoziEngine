#pragma once
#include "ngpch.h"
#include "Texture.h"
#include "ref/Ref.h"

namespace Nigozi
{
	class SubTexture {
	public:
		SubTexture() = default;
		SubTexture(const Ref<Texture>& texture, glm::vec2 size, uint32_t slotX = 0, uint32_t slotY = 0);

		void SetSubTexture(glm::vec2 size, uint32_t slotX, uint32_t slotY);
		void SetSlot(uint32_t slotX, uint32_t slotY);

		inline const Ref<Texture>& GetTexture() const { return m_texture; }

		inline uint32_t GetSlotX() { return m_slotX; }
		inline uint32_t GetSlotY() { return m_slotY; }
		inline glm::i32vec2 GetTextureSize() { return m_texture->GetSize(); }
		inline glm::vec2 GetSize() { return m_size; }
		inline const glm::vec2& GetCoordMin() { return m_coordMin; }
		inline const glm::vec2& GetCoordMax() { return m_coordMax; }
	private:
		Ref<Texture> m_texture;

		uint32_t m_slotX, m_slotY;
		glm::vec2 m_size;
		
		glm::vec2 m_coordMin;
		glm::vec2 m_coordMax;
	};
}
