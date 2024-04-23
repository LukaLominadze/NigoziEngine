#include "ngpch.h"
#include "GameObject.h"
#include "Nigozi/Layers/ObjectLayer.h"
#include "Nigozi/Layers/LayerStack.h"

namespace Nigozi 
{
	GameObject::GameObject():
		PositionX(0), PositionY(0)
	{	
		SendObject();
	}

	GameObject::GameObject(int positionX, int positionY)
	{
		PositionX = positionX;
		PositionY = positionY;

		SendObject();
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::SendObject()
	{
		static_cast<GameLayer<GameObject>*>(LayerStack::Get().GetLayer(ObjectLayer::GetInsertIndex()))->PushToVector(this);

		LOG("Object Sent!");
	}
}