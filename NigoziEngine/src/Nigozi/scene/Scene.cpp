#include "ngpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

namespace Nigozi
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
        m_Registry.clear();
    }

    void Scene::OnUpdate(float timestep)
    {
    }

    void Scene::OnRender()
    {
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto& entity : group) {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
            
            Renderer2D::DrawQuad(transform.Position, transform.Scale, sprite.Sprite, sprite.Color);
        }
    }

    void Scene::OnImGuiRender()
    {
    }

    Entity Scene::CreateEntity(const std::string& name, const std::string& tag)
    {
        Entity entity = Entity(m_Registry.create(), this);
        auto& name_ = entity.AddComponent<NameComponent>();
        name_.Name = name.empty() ? "" : name;
        auto& tag_ = entity.AddComponent<TagComponent>();
        tag_.Tag = tag.empty() ? "" : tag;
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    Entity Scene::TryGetEntityByTag(const std::string& tag)
    {
        auto entities = m_Registry.view<TagComponent>();
        for (auto entity : entities) {
            if (entities.get<TagComponent>(entity).Tag == tag) {
                return Entity(entity, const_cast<Scene*>(this));
            }
        }
        return Entity();
    }

    std::vector<Entity> Scene::TryGetEntitiesByTag(const std::string& tag)
    {
        std::vector<Entity> entities;
        auto _entities = m_Registry.view<TagComponent>();
        for (auto entity : _entities) {
            if (_entities.get<TagComponent>(entity).Tag == tag) {
                entities.push_back(Entity(entity, const_cast<Scene*>(this)));
            }
        }
        return entities;
    }
}
