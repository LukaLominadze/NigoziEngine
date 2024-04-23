#include "ngpch.h"
#include "RenderLayer.h"

namespace Nigozi
{
	std::function<uint32_t()> RenderLayer::GetInsertIndex;

	RenderLayer::RenderLayer(Renderer* renderer)
	{
		p_renderer = renderer;
	}
	

	RenderLayer::~RenderLayer()
	{
		for (Sprite* component : m_sprites) {
			delete component;
		}
	}

	void RenderLayer::SetIndex(uint32_t layerIndex)
	{
		m_layerIndex = layerIndex;
		SetGetIndexFunc([&]() { return m_layerIndex; });
	}

	void RenderLayer::OnAttach()
	{
	}

	void RenderLayer::OnUpdate()
	{
		
	}

	void RenderLayer::OnDetach()
	{
	}

	void RenderLayer::PushToVector(Sprite* obj)
	{
		m_sprites.emplace_back(obj);

		Sprite* p_sprite = m_sprites[m_vectorInstertIndex];
		p_renderer->CreateTexture(p_sprite->data);

		p_sprite = nullptr;

		m_vectorInstertIndex++;

		LOG("Component added!");
	}

	void RenderLayer::PopFromVector(Sprite* obj)
	{
		auto i = std::find(m_sprites.begin(), m_sprites.begin() + m_vectorInstertIndex, obj);
		if (i != m_sprites.begin() + m_vectorInstertIndex) {
			m_sprites.erase(i);
			m_vectorInstertIndex--;
		}
	}
}
