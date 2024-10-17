#include "ngpch.h"
#include "LayerStack.h"

namespace Nigozi
{
	LayerStack::LayerStack()
		:m_layerInsertIndex(0)
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layerStack) {
			layer->OnDetach();
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layerStack.insert(m_layerStack.begin() + m_layerInsertIndex, layer);
		layer->OnAttach();
		m_layerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_layerStack.push_back(layer);
		layer->OnAttach();
	}
}