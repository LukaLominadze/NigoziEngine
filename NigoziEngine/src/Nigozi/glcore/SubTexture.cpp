#include "ngpch.h"
#include "SubTexture.h"

Nigozi::SubTexture::SubTexture(const std::shared_ptr<Texture>& texture, glm::vec2 size, uint32_t slotX, uint32_t slotY)
	:m_size(size), m_texture(texture), m_textureSize(texture->GetWidth(), texture->GetHeight())
{
	if (m_size == glm::vec2{ 0, 0 }) {
		m_size = m_textureSize;
	}
	SetSlot(slotX, slotY);
}

void Nigozi::SubTexture::SetSubTexture(glm::vec2 size, uint32_t slotX = 0, uint32_t slotY = 0)
{
	m_size = size;
	SetSlot(slotX, slotY);
}

void Nigozi::SubTexture::SetSlot(uint32_t slotX, uint32_t slotY)
{
	m_coordMin = { slotX * m_size.x / m_textureSize.x, slotY * m_size.y / m_textureSize.y };
	m_coordMax = { (slotX + 1) * m_size.x / m_textureSize.x, (slotY + 1) * m_size.y / m_textureSize.y };
}
