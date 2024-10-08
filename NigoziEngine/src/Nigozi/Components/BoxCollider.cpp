#include "ngpch.h"
#include "BoxCollider.h"
#include "Nigozi/Layers/CollisionLayer.h"

namespace Nigozi
{
	IMPL_C_GET_INDEX_FUNC(BoxCollider, CollisionLayer)

	void BoxCollider::SetDimensions()
	{
		Transform& transform = *GetTransform();
		Vector2 position = transform.GetAbsolutePosition();
		Vector2 scale = transform.GetAbsoluteScale();

		m_rect.x = position.x;
		m_rect.y = position.y;
		m_rect.w = scale.x;
		m_rect.h = scale.y;
	}
}