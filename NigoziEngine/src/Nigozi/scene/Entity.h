#pragma once

#include "ngpch.h"
#include "Scene.h"
#include "Nigozi/core/Assert.h"
#include "Nigozi/core/Log.h"

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
				NG_CORE_LOG_CRITICAL("Component already present!");
				assert(false);
			}
			return p_scene->m_Registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() const {
			if (!HasComponent<T>()) {
				NG_CORE_LOG_CRITICAL("Component not present!");
				assert(false);
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
			p_scene->m_Registry.destroy(m_entityHandle);
		}

		inline Scene* GetScene() { return p_scene; }

	private:
		entt::entity m_entityHandle;

		Scene* p_scene;
	};
}

