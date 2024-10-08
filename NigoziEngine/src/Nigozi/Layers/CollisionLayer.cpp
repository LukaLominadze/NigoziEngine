#include "ngpch.h"
#include "CollisionLayer.h"

namespace Nigozi
{
	std::function<uint32_t()> CollisionLayer::GetInsertIndex;
	SET_INDEX(CollisionLayer)

	void CollisionLayer::OnAttach()
	{
	}

	void CollisionLayer::OnUpdate()
	{
		for (BoxCollider& collider : m_stack) {
			collider.SetDimensions();
		}
		for (size_t i = 0; i < m_stack.size(); i++) {
			for (size_t j = 0; j < i; j++) {
				if (SDL_HasIntersection(&m_stack[i].GetRect(), &m_stack[j].GetRect())) {
					std::cout << "Collided!" << std::endl;
				}
			}
			for (size_t j = i + 1; j < m_stack.size(); j++) {
				if (SDL_HasIntersection(&m_stack[i].GetRect(), &m_stack[j].GetRect())) {
					std::cout << "Collided!" << std::endl;
				}
			}
		}
	}

	void CollisionLayer::OnRender()
	{
	}

	void CollisionLayer::OnDetach()
	{
	}
}
