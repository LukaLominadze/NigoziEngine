#pragma once

#include "ngpch.h"
#include "Nigozi/Core/Core.h"

#include "Nigozi/Layers/GameLayer.h"
#include "Nigozi/GameObject.h"

namespace Nigozi
{
	class NG_API ObjectLayer : public GameLayer<GameObject>
	{
	public:
		ObjectLayer() = default;
		~ObjectLayer();

		virtual void SetIndex(uint32_t layerIndex) override;

		virtual void OnAttach() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnDetach() override;
		
		void PushToVector(GameObject* obj);
		void PopFromVector(GameObject* obj);

		BUILD_GET_INDEX_FUNC
	private:
		std::vector<GameObject*> m_objectStack;

		uint32_t m_vectorInstertIndex = 0;
	};
}

