#pragma once
#include "Nigozi/Components/Component.h"
#include "Nigozi/Core/Camera.h"

namespace Nigozi
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform() = default;

		Vector2<int> GetAbsolutePosition() const { return m_Position + Camera::Position; }
		Vector2<float> GetAbsoluteScale() const { return m_Scale * Camera::UnitSize; }

		BUILD_C_GET_INDEX_FUNC

		virtual const std::type_info& GetType() const override {
			return typeid(*this);
		}

		Vector2<int> m_Position;
		Vector2<float> m_Scale;
	};
}

