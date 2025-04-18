#pragma once

#include "ngpch.h"
#include "layers/Layer.h"

namespace Nigozi
{
	class Entity;

	class Scene : public Layer
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(float timestep) override;
		void OnRender() override;
		void OnImGuiRender() override;

		Entity CreateEntity(const std::string& name, const std::string& tag);

		friend class Entity;
	private:
		entt::registry m_Registry;
	};
}

