#include "ngpch.h"
#include "ObjectLayer.h"

namespace Nigozi
{
	std::function<uint32_t()> ObjectLayer::GetInsertIndex;

	ObjectLayer::~ObjectLayer()
	{
		for (GameObject* obj : m_objectStack) {
			delete obj;
		}
	}

	void ObjectLayer::SetIndex(uint32_t layerIndex)
	{
		m_layerIndex = layerIndex;
		SetGetIndexFunc([&]() { return m_layerIndex; });
	}

	void ObjectLayer::OnAttach()
	{
	}

	void ObjectLayer::OnUpdate()
	{
	}

	void ObjectLayer::OnDetach()
	{
	}

	void ObjectLayer::PushToVector(GameObject* component)
	{
		m_objectStack.emplace_back(component);
		m_vectorInstertIndex++;

		LOG("Object added!");

#ifdef _DEBUG
		for (GameObject* obj : m_objectStack) {
			std::cout << obj->Tag << ", ";
		}
		std::cout << std::endl;
#endif
	}

	void ObjectLayer::PopFromVector(GameObject* component)
	{
		auto i = std::find(m_objectStack.begin(), m_objectStack.begin() + m_vectorInstertIndex, component);
		if (i != m_objectStack.begin() + m_vectorInstertIndex) {
			m_objectStack.erase(i);
			m_vectorInstertIndex--;
		}
	}
}
