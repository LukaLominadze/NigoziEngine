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

	void RenderLayer::PushToVector(Component* component)
	{
		m_sprites.emplace_back(dynamic_cast<Sprite*>(component));

		Sprite* p_sprite = m_sprites[m_vectorInstertIndex];
		p_sprite->SetTexture(p_renderer->CreateTexture(p_sprite->data.spritePath));
		p_sprite = nullptr;

		m_vectorInstertIndex++;

		LOG("Component added!");
	}

	void RenderLayer::PopFromVector(Component* component)
	{
		auto i = std::find(m_sprites.begin(), m_sprites.begin() + m_vectorInstertIndex, component);
		if (i != m_sprites.begin() + m_vectorInstertIndex) {
			m_sprites.erase(i);
			m_vectorInstertIndex--;
		}
	}
}
