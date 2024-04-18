#pragma once

#include "ngpch.h"

#include "Nigozi/Core/Core.h"
#include "Nigozi/Components/Component.h"
namespace Nigozi
{
	class NG_API GameObject
	{
	public:
		GameObject();
		GameObject(int positionX, int positionY);
		~GameObject();

		template<typename T, typename... Args>
		void AddComponent(Args&&... args) {
			m_componentStack.emplace_back(new T(std::move<Args>(args)...));
		}
		template<typename T>
		void AddComponent() {
			m_componentStack.emplace_back(new T());
		}

		template<typename T>
		T* GetComponent() {
			for (Component* component : m_componentStack) {
				if (component->GetType() == typeid(T)) {
					return static_cast<T*>(component);
				}
			}
			return nullptr;
		}
	protected:
		void SendObject();
	public:
		int PositionX = 0;
		int PositionY = 0;
		std::string Tag = "";
		std::string Name = "";
	private:
		std::vector<Component*> m_componentStack;
	};
}

