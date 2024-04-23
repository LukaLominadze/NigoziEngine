#include "ngpch.h"
#include "Component.h"
#include "Nigozi/Layers/LayerStack.h"
#include "Nigozi/Layers/GameLayer.h"

namespace Nigozi
{
	template<typename T>
	void Component::SendComponent()
	{
		LOG(m_targetLayerIndex);
		static_cast<GameLayer<T>*>(LayerStack::Get().GetLayer(m_targetLayerIndex))->PushToVector(this);

		LOG("Component Sent!");
	}
}
