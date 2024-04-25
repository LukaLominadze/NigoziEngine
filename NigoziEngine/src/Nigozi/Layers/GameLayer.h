#pragma once
#include "Layer.h"

#include "ngpch.h"
#include "Nigozi/Core/Core.h"

namespace Nigozi
{
	template<typename T>
	class NG_API GameLayer : public Layer {
	public:
		template<typename... Args>
		T* CreatePushAndReturn(Args&&... args) {
			m_stack.emplace_back(T(std::forward<Args>(args)...));
			LOG("Component Created!");
			m_vectorInsertIndex++;
			return &m_stack[m_vectorInsertIndex - 1];
		}
		void Pop(uint32_t layerIndex) {
			m_stack.erase(m_stack.begin() + layerIndex);
			m_vectorInsertIndex--;
		}

		std::vector<T>& GetStack() { return m_stack; }

	protected:
		std::vector<T> m_stack;
		uint32_t m_vectorInsertIndex = 0;
	};
}