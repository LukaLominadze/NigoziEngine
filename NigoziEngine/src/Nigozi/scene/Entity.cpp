#include "ngpch.h"
#include "Entity.h"

Nigozi::Entity::Entity(entt::entity handle, Scene* scene)
	:m_entityHandle(handle), p_scene(scene)
{
}
