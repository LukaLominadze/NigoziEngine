#include "ngpch.h"
#include "GameObject.h"
#include "Nigozi/Layers/ObjectLayer.h"
#include "Nigozi/Layers/LayerStack.h"

namespace Nigozi 
{
	GameObject::GameObject()
	{	
		SendObject();
	}

	void GameObject::SendObject()
	{
		static_cast<ObjectLayer*>(LayerStack::Get().GetLayer(ObjectLayer::GetInsertIndex()))->PushToVector(this);

		LOG("Object Sent!");
	}
}