#include "ngpch.h"
#include "ObjectLayer.h"

namespace Nigozi
{
	std::function<uint32_t()> ObjectLayer::GetInsertIndex;
	SET_INDEX(ObjectLayer)

	ObjectLayer::~ObjectLayer()
	{
		for (GameObject* obj : m_objectStack) {
			delete obj;
		}
	}

	void ObjectLayer::OnAttach()
	{
	}

	void ObjectLayer::OnUpdate()
	{
	}

	void ObjectLayer::OnRender()
	{
	}

	void ObjectLayer::OnDetach()
	{
	}

	void ObjectLayer::PushToVector(GameObject* obj)
	{
		m_objectStack.emplace_back(obj);
		m_vectorInstertIndex++;

		LOG("Object added!");
	}

	void ObjectLayer::PopFromVector(GameObject* obj)
	{
		auto i = std::find(m_objectStack.begin(), m_objectStack.begin() + m_vectorInstertIndex, obj);
		if (i != m_objectStack.begin() + m_vectorInstertIndex) {
			m_objectStack.erase(i);
			m_vectorInstertIndex--;
		}
	}
}
