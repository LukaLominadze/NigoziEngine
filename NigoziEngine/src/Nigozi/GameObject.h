#pragma once

#include "ngpch.h"

#include "Nigozi/Layers/LayerStack.h"
#include "Nigozi/Layers/GameLayer.h"
#include "Nigozi/Core/Core.h"
#include "Nigozi/Components/Component.h"
#include "Nigozi/Components/Transform.h"

namespace Nigozi
{
#define GET_LAYER(type, layer_index) static_cast<GameLayer<type>*>(LayerStack::Get().GetLayer(layer_index))

	class NG_API GameObject
	{
	public:
		GameObject();
		~GameObject() = default;

		template<typename T, typename... Args>
		void AddComponent(Args&&... args) {
			m_componentStack.push_back(GET_LAYER(T, T::GetLayerIndex())->CreatePushAndReturn(std::forward<Args>(args)...));
			LOG("Recieved Component Pointer!!" << m_componentStack[m_componentStack.size() - 1]);
			LOG("Setting Trasnform... " << &m_Transform);
			m_componentStack[m_componentStack.size() - 1]->SetTransform(&m_Transform);
		}

		template<typename T>
		T* GetComponent() {
			for (Component* component : m_componentStack) {
				if (component->GetType() == T::GetType()) {
					return static_cast<T*>(component);
				}
			}
			return nullptr;
		}

		Transform m_Transform = Transform();
	protected:
		void SendObject();
	private:
		std::vector<Component*> m_componentStack;
	};
}

