#pragma once

#include "ngpch.h"
#include "layers/Layer.h"
#include "UUID.h"

class b2World;

namespace Nigozi
{
	class Entity;
	class TransformComponent;

	/*
		TODO: There probably should be different versions of a scene,
		for runtime and for the editor, since components behave differently
		in each
	*/
	class SceneTree : public Layer
	{
	public:
		SceneTree() = default;
		~SceneTree();

		void SerializeScene(const std::filesystem::path& filePath);
		void DeserializeScene(const std::filesystem::path& filePath);

		void ClearSceneTree();

		/*
			TODO: It would be good to have scene metadeta saved in a file
			and just have a LoadScene function, which would all the associated
			data to that scene. This way we also wouldn't have to create
			inherited scene classes to have different scenes
		*/
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

		TransformComponent GetWorldSpaceTransform(Entity entity) const;

		Entity CreateEntity(const std::string& name, const std::string& tag);
		Entity TryGetEntityByUUID(UUID uuid);
		Entity TryGetEntityByTag(const std::string& tag = "");
		std::vector<Entity> TryGetEntitiesByTag(const std::string& tag = "");
		bool DestroyEntity(Entity entity);

		inline UUID GetSceneRootUUID() const { return m_sceneRootUUID; }
	private:
		void SerializeNode(Entity node, nlohmann::json& doc);
	public:
		friend class Entity;
		entt::registry m_Registry;
	private:
		std::unordered_map<UUID, Entity> m_entityMap;
		UUID m_sceneRootUUID = UUID::Null;

		b2World* p_physicsWorld = nullptr;
	};
}

