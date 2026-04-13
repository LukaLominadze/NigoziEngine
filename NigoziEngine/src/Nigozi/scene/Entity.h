#pragma once

#include "ngpch.h"
#include "SceneTree.h"
#include "Nigozi/core/Assert.h"
#include "Nigozi/core/Log.h"

namespace Nigozi
{
	class Entity
	{
	public:
		Entity(entt::entity handle, SceneTree* scene);
		Entity(const Entity& other) = default;
		Entity() = default;

		UUID GetUUID();

		void SetParentUUID(UUID uuid);
		Entity GetParent();

		void SetChild(Entity entity);
		void SetChildByUUID(UUID uuid);

		std::vector<UUID>& GetChildrenUUIDs();
		std::vector<Entity> GetChildren();

		Entity GetChildByID(uint32_t id);
		Entity GetChildByUUID(UUID uuid);
		Entity GetChildByQuery(std::function<bool(Entity)> query);

		void RemoveChild(Entity entity);
		void RemoveChildByUUID(UUID uuid);


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
			if (!p_scene->m_Registry.valid(m_entityHandle)) {
				return;
			}

			if (GetUUID() == p_scene->m_sceneRootUUID) {
				p_scene->m_sceneRootUUID = UUID::Null;
			}

			SetParentUUID(UUID::Null);
			for (Entity child : GetChildren()) {
				child.Destroy();
			}

			p_scene->m_entityMap.erase(GetUUID());

			p_scene->m_Registry.destroy(m_entityHandle);
		}

		inline entt::entity GetHandle() const { return m_entityHandle; }
		inline SceneTree* GetScene() { return p_scene; }

		inline bool operator==(const Entity& other) const {
			return m_entityHandle == other.m_entityHandle;
		}
		inline bool operator!=(const Entity& other) const {
			return m_entityHandle != other.m_entityHandle;
		}
	private:
		entt::entity m_entityHandle = entt::null;

		SceneTree* p_scene;
	};
}

