#include "ngpch.h"
#include "RenderLayer.h"

namespace Nigozi
{
	std::function<uint32_t()> RenderLayer::GetInsertIndex;
	SET_INDEX(RenderLayer)	

	RenderLayer::RenderLayer(Renderer* renderer)
	{
		p_renderer = renderer;
	}


	void RenderLayer::OnAttach()
	{
	}

	void RenderLayer::OnUpdate()
	{
		for (Sprite& m_sprite : m_stack) {
			m_sprite.data.dstrect.x = m_sprite.GetTransform()->GetAbsolutePosition().x;
			m_sprite.data.dstrect.y = m_sprite.GetTransform()->GetAbsolutePosition().y;
			m_sprite.data.dstrect.w = m_sprite.GetTransform()->GetAbsoluteScale().x;
			m_sprite.data.dstrect.h = m_sprite.GetTransform()->GetAbsoluteScale().y;
		}
	}

	void RenderLayer::OnRender()
	{
		for (Sprite& m_sprite : m_stack) {
			if (m_sprite.data.p_texture == nullptr) {
				p_renderer->CreateTexture(m_sprite.data);
			}
			else {
				p_renderer->DrawTexture(&m_sprite);
			}
		}
	}

	void RenderLayer::OnDetach()
	{
	}
}
