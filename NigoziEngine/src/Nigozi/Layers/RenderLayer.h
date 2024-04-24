#pragma once

#include "ngpch.h"
#include "Nigozi/Core/Core.h"

#include "Nigozi/Layers/GameLayer.h"
#include "Nigozi/Core/Renderer.h"
#include "Nigozi/Components/Sprite.h"

namespace Nigozi
{
	class NG_API RenderLayer : public GameLayer<Sprite>
	{
	public:
		RenderLayer(Renderer* renderer);
		~RenderLayer() = default;

		virtual void SetIndex(uint32_t layerIndex) override;

		virtual void OnAttach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnDetach() override;

		BUILD_GET_INDEX_FUNC
	private:
		Renderer* p_renderer;
	};
}
