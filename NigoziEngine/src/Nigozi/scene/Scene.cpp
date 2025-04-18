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
}
