#pragma once

#include "ngpch.h"
#include "Scene.h"

namespace Nigozi
{
	class Entity
	{
	public:
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;
		Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			if (HasComponent<T>()) {
				ERROR_LOG("Component already present!");
			}
			return p_scene->m_Registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() const {
			if (HasComponent<T>()) {
				LOG("Component already present!");
				return nullptr;
			}
			return p_scene->m_Registry.get<T>(m_entityHandle);
		}

		template<typename T>
		bool HasComponent() const {
			return p_scene->m_Registry.any_of<T>(m_entityHandle);
		}

		template<typename T>
		void RemoveComponent() {
			p_scene->m_Registry.remove<T>(m_entityHandle);
		}

		void Destroy() {
			// TODO
			// The destroy function doesn't destroy the components attached to the entity
			// look through this
			p_scene->m_Registry.destroy(m_entityHandle);
		}

	private:
		entt::entity m_entityHandle;

		Scene* p_scene;
	};
}

