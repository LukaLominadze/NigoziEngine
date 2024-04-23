#pragma once

#include "ngpch.h"

#include "Nigozi/Layers/GameLayer.h"
#include "Nigozi/Core/Renderer.h"
#include "Nigozi/Components/Sprite.h"

namespace Nigozi
{
	class RenderLayer : public GameLayer<Sprite>
	{
	public:
		RenderLayer(Renderer* renderer);
		~RenderLayer();

		virtual void SetIndex(uint32_t layerIndex) override;
		virtual void OnAttach() override;
		virtual void OnUpdate() override;
		virtual void OnDetach() override;

		virtual void PushToVector(Sprite* obj) override;
		virtual void PopFromVector(Sprite* obj) override;

		std::vector<Sprite*> GetSprites() const { return m_sprites; }

		BUILD_GET_INDEX_FUNC
	private:
		Renderer* p_renderer;

		std::vector<Sprite*> m_sprites;

		uint32_t m_vectorInstertIndex = 0;
	};
}
