#pragma once

#include "ngpch.h"

#include "Layer.h"

namespace Nigozi
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		inline std::vector<Layer*>::iterator begin() { return m_layerStack.begin(); }
		inline std::vector<Layer*>::iterator end() { return m_layerStack.end(); }
	private:
		std::vector<Layer*> m_layerStack;

		uint32_t m_layerInsertIndex;
	};
}

