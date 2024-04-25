#include "ngpch.h"
#include "GameObject.h"
#include "Nigozi/Layers/ObjectLayer.h"
#include "Nigozi/Layers/LayerStack.h"
#include "Nigozi/Components/Transform.h"

namespace Nigozi 
{
	GameObject::GameObject()
	{	
		SendObject();
		AddComponent<Transform>();
	}

	void GameObject::SendObject()
	{
		static_cast<ObjectLayer*>(LayerStack::Get().GetLayer(ObjectLayer::GetInsertIndex()))->PushToVector(this);

		LOG("Object Sent!");
	}
}