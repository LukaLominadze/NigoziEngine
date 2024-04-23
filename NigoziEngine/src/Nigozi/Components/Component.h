#pragma once

#include "Nigozi/Core/Core.h"

#include "ngpch.h"

namespace Nigozi
{
	class NG_API Component {
	public:
		virtual void SetTargetIndex() {}
		virtual const std::type_info& GetType() const {
			return typeid(*this);
		}
	protected:
		uint32_t m_targetLayerIndex = 0;
	};

#define IMPL_TARGET_INDEX_FUNC(this_class, target_class) void this_class::SetTargetIndex() { \
																m_targetLayerIndex = target_class::GetInsertIndex(); \
														 }

#define SEND_COMPONENT(type, component) static_cast<GameLayer<type>*>(LayerStack::Get().GetLayer(m_targetLayerIndex))->PushToVector(component);
}