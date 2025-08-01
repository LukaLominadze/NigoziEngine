#include "ngpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Component.h"

namespace Nigozi
{
    Scene::Scene(SceneManager* sceneManager)
        :p_sceneManager(sceneManager)
    {
    }

    Scene::~Scene()
    {
        m_Registry.clear();
    }

    void Scene::OnEvent(Event& event)
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            script.ScriptHandle->OnEvent(event);
        }
    }

    void Scene::OnUpdate(float timestep)
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        scriptView.each([timestep](auto script) {
            script.ScriptHandle->OnUpdate(timestep);
            });
    }

    void Scene::OnRender()
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        scriptView.each([](auto script) {
            script.ScriptHandle->OnRender();
            });

        m_Registry.sort<SpriteRendererComponent>([](const SpriteRendererComponent& a, const SpriteRendererComponent& b) {
            return a.ZOrder < b.ZOrder;
            });

        auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        view.use<SpriteRendererComponent>();
        view.each([](auto entity, auto& transform, auto& sprite) {
            Renderer2D::DrawRotatedQuad(transform.Position, transform.Scale, glm::radians(transform.Rotation), sprite.Sprite, sprite.Color);
            });
    }

    void Scene::OnImGuiRender()
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        scriptView.each([](auto script) {
            script.ScriptHandle->OnImGuiRender();
            });
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
