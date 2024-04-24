#pragma once

#include "ngpch.h"

#include "Nigozi/Core/Core.h"

#include "Nigozi/Components/Component.h"
#include "Nigozi/Layers/Layer.h"

namespace Nigozi
{
	class NG_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_layerStack.begin(); }
		std::vector<Layer*>::iterator end() { return m_layerStack.end(); }

		inline Layer* GetLayer(uint32_t layerIndex) { return m_layerStack[layerIndex]; }

		inline static LayerStack& Get() { return *s_Instance; }
	private:
		std::vector<Layer*> m_layerStack;

		static LayerStack* s_Instance;

		uint32_t m_layerInsertIndex = 0;
	};
}
