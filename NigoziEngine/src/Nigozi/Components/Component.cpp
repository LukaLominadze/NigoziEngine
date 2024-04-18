#include "ngpch.h"
#include "Component.h"
#include "Nigozi/Layers/LayerStack.h"

void Nigozi::Component::SendComponent()
{
	LOG(m_targetLayerIndex);
	LayerStack::Get().GetLayer(m_targetLayerIndex)->PushToVector(this);

	LOG("Component Sent!");
}
