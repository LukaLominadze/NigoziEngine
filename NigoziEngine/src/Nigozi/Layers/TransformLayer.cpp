#include "ngpch.h"
#include "TransformLayer.h"

namespace Nigozi
{
	std::function<uint32_t()> TransformLayer::GetInsertIndex;
	SET_INDEX(TransformLayer)

	void TransformLayer::OnAttach()
	{
	}

	void TransformLayer::OnUpdate()
	{
	}

	void TransformLayer::OnRender()
	{
	}

	void TransformLayer::OnDetach()
	{
	}
}