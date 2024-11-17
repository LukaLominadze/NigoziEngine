#include "ngpch.h"
#include "SubTexture.h"

Nigozi::SubTexture::SubTexture(const std::shared_ptr<Texture>& texture, float size, uint32_t slotX, uint32_t slotY)
	:m_size(size), m_texture(texture), m_textureSize(texture->GetWidth(), texture->GetHeight())
{
	SetSlot(slotX, slotY);
}

void Nigozi::SubTexture::SetSubTexture(float size)
{
	m_size = size;
	SetSlot(0, 0);
}

void Nigozi::SubTexture::SetSubTexture(float size, uint32_t slotX, uint32_t slotY)
{
	m_size = size;
	SetSlot(slotX, slotY);
}

void Nigozi::SubTexture::SetSlot(uint32_t slotX, uint32_t slotY)
{
	m_coordMin = { slotX * m_size / m_textureSize.x, slotY * m_size / m_textureSize.y };
	m_coordMax = { (slotX + 1) * m_size / m_textureSize.x, (slotY + 1) * m_size / m_textureSize.y };
}
