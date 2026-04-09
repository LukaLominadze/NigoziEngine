#include "ngpch.h"
#include "SceneTree.h"
#include "Entity.h"
#include "Component.h"

namespace Nigozi
{
    SceneTree::~SceneTree()
    {
        m_Registry.clear();
    }

    void SceneTree::OnEvent(Event& event)
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            script.ScriptHandle->OnEvent(event);
        }
    }

    void SceneTree::OnUpdate(float timestep)
    {
        m_Registry.sort<UUIDComponent>([&](const auto lhs, const auto rhs) {
            auto lhsEntity = m_entityMap.find(lhs.ID)->second;
            auto rhsEntity = m_entityMap.find(rhs.ID)->second;
            return lhsEntity.GetHandle() < rhsEntity.GetHandle();
            });

        auto audioView = m_Registry.view<AudioStreamPlayerComponent>();
        audioView.each([](auto audio) {
            audio.AudioHandle->Update();
            });

        auto scriptView = m_Registry.view<ScriptComponent>();
        scriptView.each([timestep](auto script) {
            script.ScriptHandle->OnUpdate(timestep);
            });
    }

    void SceneTree::OnRender()
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
            glm::vec2 spriteSize = sprite.Sprite.GetSize();
            float sizeY = spriteSize.y / sprite.Sprite.GetTextureSize().y;
            float aspectX = spriteSize.x / spriteSize.y;
            glm::vec2 scale(transform.Scale.x * aspectX * sizeY, transform.Scale.y * sizeY);
            Renderer2D::DrawRotatedQuad(transform.Position, scale, glm::radians(transform.Rotation), sprite.Sprite, sprite.Color);
            });
    }

    void SceneTree::OnImGuiRender()
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        scriptView.each([](auto script) {
            script.ScriptHandle->OnImGuiRender();
            });
    }

    void SceneTree::OnEditorEvent(Event& event)
    {

    }

    void SceneTree::OnEditorUpdate(float timestep)
    {
        auto audioView = m_Registry.view<AudioStreamPlayerComponent>();
        audioView.each([](auto audio) {
            audio.AudioHandle->Update();
            });
    }

    void SceneTree::OnEditorRender()
    {
        m_Registry.sort<SpriteRendererComponent>([](const SpriteRendererComponent& a, const SpriteRendererComponent& b) {
            return a.ZOrder < b.ZOrder;
            });

        auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
        view.use<SpriteRendererComponent>();
        view.each([&](auto entity, auto& transform, auto& sprite) {
            auto worldTransform = GetWorldSpaceTransform(Entity(entity, this));

            glm::vec2 spriteSize = sprite.Sprite.GetSize();
            float sizeY = spriteSize.y / sprite.Sprite.GetTextureSize().y;
            float aspectX = spriteSize.x / spriteSize.y;
            glm::vec2 scale(worldTransform.Scale.x * aspectX * sizeY, worldTransform.Scale.y * sizeY);
            Renderer2D::DrawRotatedQuad(worldTransform.Position, scale, glm::radians(worldTransform.Rotation), sprite.Sprite, sprite.Color);
            });
    }

    void SceneTree::OnEditorImGuiRender()
    {

    }

    TransformComponent SceneTree::GetWorldSpaceTransform(Entity entity) const
    {
        TransformComponent transform;

        float localRotation = entity.GetComponent<TransformComponent>().Rotation;

        Entity parent = entity.GetParent();
        if (parent != Entity()) {
            transform = GetWorldSpaceTransform(parent);
        }

        TransformComponent thisTransform = entity.GetComponent<TransformComponent>();
        thisTransform.Rotation += transform.Rotation;
        thisTransform.Scale *= transform.Scale;

        float cosR = glm::cos(glm::radians(thisTransform.Rotation - localRotation));
        float sinR = glm::sin(-glm::radians(thisTransform.Rotation - localRotation));

        glm::vec2 rotatedOffset;
        rotatedOffset.x = (thisTransform.Position.x * transform.Scale.x * cosR) - (thisTransform.Position.y * transform.Scale.y * sinR);
        rotatedOffset.y = (thisTransform.Position.x * transform.Scale.x * sinR) + (thisTransform.Position.y * transform.Scale.y * cosR);

        thisTransform.Position = transform.Position + rotatedOffset;

        return thisTransform;
    }

    Entity SceneTree::CreateEntity(const std::string& name, const std::string& tag)
    {
        Entity entity = Entity(m_Registry.create(), this);
        auto& uuid = entity.AddComponent<Nigozi::UUIDComponent>();

        if (m_entityMap.empty()) {
            entity.AddComponent<SceneComponent>();
            m_sceneRootUUID = uuid.ID;
        }
        m_entityMap[uuid.ID] = entity;

        auto& name_ = entity.AddComponent<NameComponent>();
        name_.Name = name.empty() ? "" : name;

        auto& tag_ = entity.AddComponent<TagComponent>();
        tag_.Tag = tag.empty() ? "" : tag;

        entity.AddComponent<TransformComponent>();
        entity.AddComponent<RelationshipComponent>();

        return entity;
    }

    Entity SceneTree::TryGetEntityByUUID(UUID uuid)
    {
        if (m_entityMap.find(uuid) != m_entityMap.end()) {
            return m_entityMap.at(uuid);
        }
        return Entity();
    }

    Entity SceneTree::TryGetEntityByTag(const std::string& tag)
    {
        auto entities = m_Registry.view<TagComponent>();
        for (auto entity : entities) {
            if (entities.get<TagComponent>(entity).Tag == tag) {
                return Entity(entity, const_cast<SceneTree*>(this));
            }
        }
        return Entity();
    }

    std::vector<Entity> SceneTree::TryGetEntitiesByTag(const std::string& tag)
    {
        std::vector<Entity> entities;
        auto _entities = m_Registry.view<TagComponent>();
        for (auto entity : _entities) {
            if (_entities.get<TagComponent>(entity).Tag == tag) {
                entities.push_back(Entity(entity, const_cast<SceneTree*>(this)));
            }
        }
        return entities;
    }

    bool SceneTree::DestroyEntity(Entity entity)
    {
        if (!m_Registry.valid(entity.GetHandle())) {
            return false;
        }

        if (entity.GetUUID() == m_sceneRootUUID) {
            m_sceneRootUUID = UUID::Null;
        }

        entity.SetParentUUID(UUID::Null);
        for (Entity child : entity.GetChildren()) {
            DestroyEntity(child);
        }

        m_Registry.destroy(entity.GetHandle());
        return true;
    }
}
