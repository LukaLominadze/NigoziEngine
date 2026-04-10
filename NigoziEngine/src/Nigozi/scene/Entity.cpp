#include "ngpch.h"
#include "Entity.h"
#include "Component.h"

namespace Nigozi
{
	Entity::Entity(entt::entity handle, SceneTree* scene)
		:m_entityHandle(handle), p_scene(scene)
	{
	}

	UUID Entity::GetUUID()
	{
		return GetComponent<UUIDComponent>().ID;
	}

	void Entity::SetParentUUID(UUID uuid)
	{
		auto& relationship = GetComponent<RelationshipComponent>();
		UUID thisUuid = GetUUID();

		if (relationship.ParentUUID.GetUUID() != Nigozi::UUID::Null) {
			std::vector<UUID>& childrenUUIDs = GetParent().GetComponent<RelationshipComponent>().ChildrenUUIDs;
			auto it = std::find(childrenUUIDs.begin(), childrenUUIDs.end(), thisUuid);
			if (it != childrenUUIDs.end()) {
				childrenUUIDs.erase(std::find(childrenUUIDs.begin(), childrenUUIDs.end(), thisUuid));
			}
		}

		relationship.ParentUUID = uuid;

		if (uuid == UUID(UUID::Null)) {
			return;
		}
		
		std::vector<UUID>& childrenUUIDs = GetParent().GetComponent<RelationshipComponent>().ChildrenUUIDs;
		childrenUUIDs.push_back(thisUuid);
	}

	Entity Entity::GetParent()
	{
		return p_scene->TryGetEntityByUUID(GetComponent<RelationshipComponent>().ParentUUID);
	}

	void Entity::SetChild(Entity entity)
	{
		UUID uuid = entity.GetUUID();
		std::vector<UUID>& childrenUUIDs = GetChildrenUUIDs();
		
		auto it = std::find(childrenUUIDs.begin(), childrenUUIDs.end(), uuid);
		if (it != childrenUUIDs.end()) {
			return;
		}

		childrenUUIDs.push_back(uuid);
	}

	void Entity::SetChildByUUID(UUID uuid)
	{
		std::vector<UUID>& childrenUUIDs = GetChildrenUUIDs();

		auto it = std::find(childrenUUIDs.begin(), childrenUUIDs.end(), uuid);
		if (it != childrenUUIDs.end()) {
			return;
		}

		childrenUUIDs.push_back(uuid);
	}

	std::vector<UUID>& Entity::GetChildrenUUIDs()
	{
		return GetComponent<RelationshipComponent>().ChildrenUUIDs;
	}

	std::vector<Entity> Entity::GetChildren()
	{
		const std::vector<UUID>& childrenUUIDs = GetComponent<RelationshipComponent>().ChildrenUUIDs;
		std::vector<Entity> children;
		children.reserve(childrenUUIDs.size());
		for (const UUID uuid : childrenUUIDs) {
			children.push_back(p_scene->TryGetEntityByUUID(uuid));
		}
		return children;
	}

	Entity Entity::GetChildByID(uint32_t id)
	{
		Entity child{};

		const std::vector<UUID>& childrenUUIDs = GetComponent<RelationshipComponent>().ChildrenUUIDs;
		if (childrenUUIDs.size() < id) {
			child = p_scene->TryGetEntityByUUID(childrenUUIDs[id]);
		}

		return child;
	}

	Entity Entity::GetChildByUUID(UUID uuid)
	{
		Entity child{};

		const std::vector<UUID>& childrenUUIDs = GetComponent<RelationshipComponent>().ChildrenUUIDs;
		for (const UUID _uuid : childrenUUIDs) {
			if (_uuid == uuid) {
				child = p_scene->TryGetEntityByUUID(uuid);
				break;
			}
		}

		return child;
	}

	Entity Entity::GetChildByQuery(std::function<bool(Entity)> query)
	{
		Entity child{};

		const std::vector<UUID>& childrenUUIDs = GetComponent<RelationshipComponent>().ChildrenUUIDs;
		for (const UUID uuid : childrenUUIDs) {
			Entity entity = p_scene->TryGetEntityByUUID(uuid);
			if (query(entity)) {
				child = entity;
				break;
			}
		}

		return child;
	}

	void Entity::RemoveChild(Entity entity)
	{
		UUID uuid = entity.GetUUID();
		std::vector<UUID>& childrenUUIDs = GetChildrenUUIDs();

		auto it = std::find(childrenUUIDs.begin(), childrenUUIDs.end(), uuid);
		if (it == childrenUUIDs.end()) {
			return;
		}

		childrenUUIDs.erase(it);

		auto& relationship = entity.GetComponent<RelationshipComponent>();
		relationship.ParentUUID = UUID::Null;
	}

	void Entity::RemoveChildByUUID(UUID uuid)
	{
		std::vector<UUID>& childrenUUIDs = GetChildrenUUIDs();

		auto it = std::find(childrenUUIDs.begin(), childrenUUIDs.end(), uuid);
		if (it == childrenUUIDs.end()) {
			return;
		}

		childrenUUIDs.erase(it);

		auto& relationship = p_scene->TryGetEntityByUUID(uuid).GetComponent<RelationshipComponent>();
		relationship.ParentUUID = UUID::Null;
	}

}