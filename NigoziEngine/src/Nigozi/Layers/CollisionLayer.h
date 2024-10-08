#pragma once
#include "Nigozi/Layers/GameLayer.h"
#include "Nigozi/Components/BoxCollider.h"

namespace Nigozi
{
	class NG_API CollisionLayer : public GameLayer<BoxCollider>
	{
	public:
		CollisionLayer() = default;
		~CollisionLayer() = default;

		virtual void SetIndex(uint32_t layerIndex) override;

		virtual void OnAttach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnDetach() override;

		BUILD_GET_INDEX_FUNC
	};
}

