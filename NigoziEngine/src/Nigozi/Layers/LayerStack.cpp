#include "ngpch.h"
#include "LayerStack.h"

namespace Nigozi
{
	NG_API LayerStack* LayerStack::s_Instance = nullptr;

	LayerStack::LayerStack()
	{
		if (!s_Instance) {
			s_Instance = this;
		}
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layerStack) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layerStack.emplace(m_layerStack.begin() + m_layerInsertIndex, layer);
		layer->SetIndex(m_layerInsertIndex);
		layer->OnAttach();
		m_layerInsertIndex++;
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layerStack.begin(), m_layerStack.begin() + m_layerInsertIndex, layer);
		if (it != m_layerStack.begin() + m_layerInsertIndex) {
			layer->OnDetach();
			m_layerStack.erase(it);
			m_layerInsertIndex--;

			int index = std::distance(m_layerStack.begin(), it);

			for (int i = index; i < m_layerStack.size(); ++i) {
				m_layerStack[i]->SetIndex(i);
			}
		}
	}
}