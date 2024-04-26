#pragma once
#include "Nigozi/Components/Component.h"
#include "Nigozi/Core/Camera.h"

namespace Nigozi
{
	struct Transform
	{
		Vector2<float> GetAbsolutePosition() const { return (Position + Camera::Position) * Camera::UnitSize; }
		Vector2<float> GetAbsoluteScale() const { return Scale * Camera::UnitSize; }

		Vector2<float> Position = Vector2<float>(0, 0);
		Vector2<float> Scale = Vector2<float>(1, 1);
	};
}
