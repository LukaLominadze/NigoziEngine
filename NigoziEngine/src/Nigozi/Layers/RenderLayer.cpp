#include "ngpch.h"
#include "RenderLayer.h"

#include "Nigozi/Components/Transform.h"

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
			Transform* p_transform = m_sprite.GetTransform();
			Vector2<float> position = p_transform->GetAbsolutePosition();
			Vector2<float> scale = p_transform->GetAbsoluteScale();

			m_sprite.data.dstrect.x = position.x;
			m_sprite.data.dstrect.y = position.y;
			m_sprite.data.dstrect.w = scale.x;
			m_sprite.data.dstrect.h = scale.y;
			
			p_transform = nullptr;
		}
	}

	void RenderLayer::OnRender()
	{
		for (Sprite& m_sprite : m_stack) {
			if (m_sprite.data.p_texture == nullptr) {
				p_renderer->CreateTexture(m_sprite.data);
			}
			p_renderer->DrawTexture(m_sprite);
		}
	}

	void RenderLayer::OnDetach()
	{
	}
}
