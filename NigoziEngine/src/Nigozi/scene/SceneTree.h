#pragma once

#include "ngpch.h"
#include "layers/Layer.h"
#include "UUID.h"

class b2World;

namespace Nigozi
{
	class Entity;
	class TransformComponent;
	class CameraComponent;

	class SceneTree : public Layer
	{
	public:
		SceneTree() = default;
		~SceneTree();

		bool SerializeScene();
		bool SerializeScene(const std::filesystem::path& filePath);
		void DeserializeScene(const std::filesystem::path& filePath);

		void ClearSceneTree();

		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& event) override;
		void OnUpdate(float timestep) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnEditorEvent(Event& event);
		void OnEditorUpdate(float timestep);
		void OnEditorRender();
		void OnEditorImGuiRender();

		inline const std::filesystem::path GetFilePath() const { return m_filePath; }
		inline bool HasFilePath() const { return !m_filePath.empty(); }

		TransformComponent GetWorldSpaceTransform(Entity entity) const;

		Entity CreateEntity(const std::string& name, const std::string& tag, UUID uuid = UUID());
		Entity TryGetEntityByUUID(UUID uuid);
		Entity TryGetEntityByTag(const std::string& tag = "");
		std::vector<Entity> TryGetEntitiesByTag(const std::string& tag = "");
		void QueueDestroyEntity(Entity entity);
		bool DestroyEntity(Entity entity);

		CameraComponent* GetMainCamera();

		inline UUID GetSceneRootUUID() const { return m_sceneRootUUID; }
	private:
		void SerializeNode(Entity node, YAML::Node& doc);
	public:
		friend class Entity;
		entt::registry m_Registry;
	private:
		std::unordered_map<UUID, Entity> m_entityMap;
		std::vector<Nigozi::Entity> m_destroyQueue;
		UUID m_sceneRootUUID = UUID::Null;

		std::filesystem::path m_filePath;

		b2World* p_physicsWorld = nullptr;
	};
}

