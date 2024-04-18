#pragma once

#include "ngpch.h"

#include "Nigozi/Components/Component.h"
#include "Nigozi/GameObject.h"

namespace Nigozi
{
	class Layer {
	public:
		virtual void SetIndex(uint32_t layerIndex) = 0;
		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDetach() = 0;

		inline void SetLayerIndex(uint32_t layerIndex) { m_layerIndex = layerIndex; };

		virtual void PushToVector(Component* component) {};
		virtual void PopFromVector(Component* component) {};
		virtual void PushToVector(GameObject* component) {};
		virtual void PopFromVector(GameObject* component) {};
	protected:
		uint32_t m_layerIndex;
	};

#define BUILD_GET_INDEX_FUNC static void SetGetIndexFunc(std::function<uint32_t()> func) { \
								GetInsertIndex = func; \
							 } \
							 static std::function<uint32_t()> GetInsertIndex;
}