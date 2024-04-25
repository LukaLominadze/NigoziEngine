#include "ngpch.h"
#include "Transform.h"
#include "Nigozi/Layers/TransformLayer.h"

namespace Nigozi
{
	IMPL_C_GET_INDEX_FUNC(Transform, TransformLayer)

	Transform::Transform() :
		m_Position(0, 0), m_Scale(1, 1)
	{
	}
}