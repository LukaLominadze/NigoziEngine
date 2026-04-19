#include "ngpch.h"
#include "SceneTree.h"
#include "Entity.h"
#include "Component.h"
#include "scripts/ScriptEngine.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace Nigozi
{
    SceneTree::~SceneTree()
    {
        m_Registry.clear();
        m_entityMap.clear();
    }

    bool SceneTree::SerializeScene()
    {
        return SerializeScene(m_filePath);
    }

    bool SceneTree::SerializeScene(const std::filesystem::path& filePath)
    {
        if (filePath.empty()) {
            return false;
        }

        Entity entity = TryGetEntityByUUID(m_sceneRootUUID);
        if (entity == Entity()) {
            return false;
        }

        entity.GetComponent<SceneComponent>().FilePath = filePath;

        NG_CORE_LOG_INFO("[Scene Tree] Serializing scene at: {}", filePath.string());

        constexpr uint64_t SERIALIZER_VERSION = 0;

        YAML::Node data;

        data["Metadata"]["Version"] = SERIALIZER_VERSION;
        YAML::Node nodes;

        SerializeNode(entity, nodes);
        data["Nodes"] = nodes;

        std::ofstream file(filePath);

        // set indent to 4
        file << std::setw(4) << data << std::endl;

        m_filePath = filePath;

        return true;
    }

    void SceneTree::DeserializeScene(const std::filesystem::path& filePath)
    {
        if (filePath.empty() || !std::filesystem::exists(filePath)) {
            return;
        }

        NG_CORE_LOG_INFO("[Scene Tree] Derializing scene from: {}", filePath.string());

        YAML::Node data = YAML::LoadFile(filePath.string());
        if (!data["Nodes"]) {
            return;
        }

        for (const auto& ent : data["Nodes"]) {
            if (ent["Scene"] && !m_entityMap.empty()) {
                std::filesystem::path scenePath(ent["Scene"]["FilePath"].as<std::string>());
                DeserializeScene(scenePath);
            }

            Entity entity = Entity(m_Registry.create(), this);

            auto& uuid = entity.AddComponent<Nigozi::UUIDComponent>();
            uuid.ID = UUID(ent["UUID"]["Id"].as<uint64_t>());

            auto& name = entity.AddComponent<NameComponent>();
            name.Name = ent["Name"]["Name"].as<std::string>();

            auto& tag = entity.AddComponent<TagComponent>();
            tag.Tag = ent["Tag"]["Tag"].as<std::string>();

            auto& nodeType = entity.AddComponent<NodeTypeComponent>();
            nodeType.Type = NodeTypeComponent::Types::Node;

            if (ent["Scene"]) {
                auto& scene = entity.AddComponent<SceneComponent>();
                nodeType.Type = NodeTypeComponent::Types::Scene;
                if (ent["Scene"]["FilePath"])
                    scene.FilePath = ent["Scene"]["FilePath"].as<std::string>();
                if (m_entityMap.empty()) {
                    m_sceneRootUUID = uuid.ID;
                }
            }
            m_entityMap[uuid.ID] = entity;

            if (ent["Script"]) {
                auto& script = entity.AddComponent<ScriptComponent>();
                script.ScriptPath = ent["Script"]["FilePath"].as<std::string>();
            }

            auto& transform = entity.AddComponent<TransformComponent>();
            transform.Position = glm::vec2(
                ent["Transform"]["Position"]["X"].as<float>(),
                ent["Transform"]["Position"]["Y"].as<float>()
            );
            transform.Scale = glm::vec2(
                ent["Transform"]["Scale"]["X"].as<float>(),
                ent["Transform"]["Scale"]["Y"].as<float>()
            );
            transform.Rotation = ent["Transform"]["Rotation"].as<float>();

            auto& relationship = entity.AddComponent<RelationshipComponent>();
            relationship.ParentUUID = ent["Relationship"]["ParentUUID"].as<uint64_t>();

            if (relationship.ParentUUID.GetUUID() != UUID::Null) {
                entity.SetParentUUID(relationship.ParentUUID);
            }

            if (ent["Camera"]) {
                auto& camera = entity.AddComponent<CameraComponent>();
                nodeType.Type = NodeTypeComponent::Types::Camera;
                if (ent["Camera"]["Current"])
                    camera.Current = ent["Camera"]["Current"].as<bool>();

                if (ent["Camera"]["Aspect"])
                    camera.Aspect = ent["Camera"]["Aspect"].as<float>();

                if (ent["Camera"]["Zoom"])
                    camera.Zoom = ent["Camera"]["Zoom"].as<float>();
            }

            if (ent["Sprite"]) {
                auto& sprite = entity.AddComponent<SpriteRendererComponent>();
                nodeType.Type = NodeTypeComponent::Types::SpriteRenderer;

                std::string texturePath;
                if (ent["Sprite"]["Texture"] && ent["Sprite"]["Texture"]["FilePath"]) {
                    texturePath = ent["Sprite"]["Texture"]["FilePath"].as<std::string>();
                }
                if (std::filesystem::exists(std::filesystem::path(texturePath))) {
                    sprite.SpriteTexture = Ref<Texture>::Create(texturePath);
                }
                else {
                    sprite.SpriteTexture = Renderer2D::GetData()->Textures[0];
                    NG_CORE_LOG_ERROR("[SceneTree] Couldn't find texture with path: {}", texturePath);
                }

                glm::vec4 color;
                if (ent["Sprite"]["Color"]) {
                    color.x = ent["Sprite"]["Color"]["R"].as<float>();
                    color.y = ent["Sprite"]["Color"]["G"].as<float>();
                    color.z = ent["Sprite"]["Color"]["B"].as<float>();
                    color.w = ent["Sprite"]["Color"]["A"].as<float>();
                }

                glm::u32vec2 seperator(1);
                if (ent["Sprite"]["Seperator"]) {
                    seperator = glm::u32vec2(
                        ent["Sprite"]["Seperator"]["X"].as<int32_t>(),
                        ent["Sprite"]["Seperator"]["Y"].as<int32_t>()
                    );
                }

                glm::vec2 textureSize = sprite.SpriteTexture->GetSize();
                glm::vec2 subTextureSize(
                    textureSize.x / seperator.x,
                    textureSize.y / seperator.y
                );

                glm::u32vec2 slot(0);
                if (ent["Sprite"]["Slot"]) {
                    slot = glm::u32vec2(
                        ent["Sprite"]["Slot"]["X"].as<uint32_t>(),
                        ent["Sprite"]["Slot"]["Y"].as<uint32_t>()
                    );
                }

                sprite.Sprite = SubTexture(sprite.SpriteTexture, subTextureSize, slot.x, slot.y);
                sprite.Color = color;
            }

            if (ent["Rigidbody"]) {
                auto& rigidbody = entity.AddComponent<RigidbodyComponent>();
                nodeType.Type = NodeTypeComponent::Types::Rigidbody;

                if (ent["Rigidbody"]["BodyType"]) {
                    rigidbody.Type = (RigidbodyComponent::BodyType)ent["Rigidbody"]["BodyType"].as<int32_t>();
                }
                if (ent["Rigidbody"]["FreezeRotation"]) {
                    rigidbody.FreezeRotation = ent["Rigidbody"]["FreezeRotation"].as<bool>();
                }
            }

            if (ent["BoxCollider"]) {
                auto& boxCollider = entity.AddComponent<BoxColliderComponent>();
                if (ent["BoxCollider"]["Size"]) {
                    boxCollider.Size.x = ent["BoxCollider"]["Size"]["X"].as<float>();
                    boxCollider.Size.y = ent["BoxCollider"]["Size"]["Y"].as<float>();
                }
                if (ent["BoxCollider"]["Density"]) {
                    boxCollider.Density = ent["BoxCollider"]["Density"].as<float>();
                }
                if (ent["BoxCollider"]["Friction"]) {
                    boxCollider.Friction = ent["BoxCollider"]["Friction"].as<float>();
                }
                if (ent["BoxCollider"]["Restitution"]) {
                    boxCollider.Restitution = ent["BoxCollider"]["Restitution"].as<float>();
                }
                if (ent["BoxCollider"]["RestitutionThreshold"]) {
                    boxCollider.RestitutionThreshold = ent["BoxCollider"]["RestitutionThreshold"].as<float>();
                }
            }

            if (ent["AudioStreamPlayer"]) {
                auto& audio = entity.AddComponent<AudioStreamPlayerComponent>();
                nodeType.Type = NodeTypeComponent::Types::AudioStreamPlayer;

                std::filesystem::path audioPath;
                if (ent["AudioStreamPlayer"]["FilePath"]) {
                    audioPath = ent["AudioStreamPlayer"]["FilePath"].as<std::string>();
                }
                if (std::filesystem::exists(audioPath)) {
                    audio.AudioHandle = AudioEngine::LoadAudioFromFile(audioPath);
                }
                else {
                    NG_CORE_LOG_ERROR("[Scene Tree] Couldn't find audio file with path: {}", audioPath.string());
                }

                if (ent["AudioStreamPlayer"]["Volume"]) {
                    audio.Volume = ent["AudioStreamPlayer"]["Volume"].as<float>();
                }
            }
        }

        m_filePath = filePath;
    }

    void SceneTree::ClearSceneTree()
    {
        m_Registry.clear();
        m_entityMap.clear();
        m_sceneRootUUID = UUID(UUID::Null);
        NG_CORE_LOG_INFO("[Scene Tree] Cleared tree");
    }

    void SceneTree::SerializeNode(Entity node, YAML::Node& doc)
    {
        YAML::Node ent;

        auto& uuid = node.GetComponent<UUIDComponent>();
        ent["UUID"]["Id"] = uuid.ID.GetUUID();

        auto& name = node.GetComponent<NameComponent>();
        ent["Name"]["Name"] = name.Name;

        auto& tag = node.GetComponent<TagComponent>();
        ent["Tag"]["Tag"] = tag.Tag;

        auto& nodeType = node.GetComponent<NodeTypeComponent>();
        ent["Type"] = (uint32_t)nodeType.Type;

        if (node.HasComponent<SceneComponent>()) {
            auto& scene = node.GetComponent<SceneComponent>();
            ent["Scene"]["FilePath"] = std::filesystem::absolute(scene.FilePath).string();
            if (uuid.ID != m_sceneRootUUID) {
                // if we are using an external scene, stop serializing
                doc.push_back(ent);
                return;
            }
        }

        auto& transform = node.GetComponent<TransformComponent>();
        ent["Transform"]["Position"]["X"] = transform.Position.x;
        ent["Transform"]["Position"]["Y"] = transform.Position.y;

        ent["Transform"]["Scale"]["X"] = transform.Scale.x;
        ent["Transform"]["Scale"]["Y"] = transform.Scale.y;

        ent["Transform"]["Rotation"] = transform.Rotation;

        auto& relationship = node.GetComponent<RelationshipComponent>();

        // children data will not be saved
        // parent data is enough to load and save entities
        ent["Relationship"]["ParentUUID"] = relationship.ParentUUID.GetUUID();

        if (node.HasComponent<CameraComponent>()) {
            auto& camera = node.GetComponent<CameraComponent>();
            ent["Camera"]["Current"] = camera.Current;
            ent["Camera"]["aspect"] = camera.Aspect;
            ent["Camera"]["zoom"] = camera.Zoom;
        }

        if (node.HasComponent<ScriptComponent>()) {
            auto& script = node.GetComponent<ScriptComponent>();
            ent["Script"]["FilePath"] = script.ScriptPath.string();
        }

        if (node.HasComponent<SpriteRendererComponent>()) {
            auto& sprite = node.GetComponent<SpriteRendererComponent>();
            ent["Sprite"]["Texture"]["FilePath"] = sprite.SpriteTexture->GetPath();

            ent["Sprite"]["Color"]["R"] = sprite.Color.x;
            ent["Sprite"]["Color"]["G"] = sprite.Color.y;
            ent["Sprite"]["Color"]["B"] = sprite.Color.z;
            ent["Sprite"]["Color"]["A"] = sprite.Color.w;

            ent["Sprite"]["Slot"]["X"] = sprite.Sprite.GetSlotX();
            ent["Sprite"]["Slot"]["Y"] = sprite.Sprite.GetSlotY();

            glm::i32vec2 seperators = glm::i32vec2((glm::vec2)sprite.Sprite.GetTextureSize() / sprite.Sprite.GetSize());
            if (seperators.x == 0)
                seperators.x++;
            if (seperators.y == 0)
                seperators.y++;
            ent["Sprite"]["Seperator"]["X"] = (int32_t)(seperators.x);
            ent["Sprite"]["Seperator"]["Y"] = (int32_t)(seperators.y);
        }

        if (node.HasComponent<RigidbodyComponent>()) {
            auto& rigidbody = node.GetComponent<RigidbodyComponent>();
            ent["Rigidbody"]["BodyType"] = (int32_t)rigidbody.Type;
            ent["Rigidbody"]["FreezeRotation"] = rigidbody.FreezeRotation;
        }

        if (node.HasComponent<BoxColliderComponent>()) {
            auto& boxCollider = node.GetComponent<BoxColliderComponent>();
            ent["BoxCollider"]["Size"]["X"] = boxCollider.Size.x;
            ent["BoxCollider"]["Size"]["Y"] = boxCollider.Size.y;

            ent["BoxCollider"]["Density"] = boxCollider.Density;
            ent["BoxCollider"]["Friction"] = boxCollider.Friction;
            ent["BoxCollider"]["Restitution"] = boxCollider.Restitution;
            ent["BoxCollider"]["RestitutionThreshold"] = boxCollider.RestitutionThreshold;
        }

        if (node.HasComponent<AudioStreamPlayerComponent>()) {
            auto& audio = node.GetComponent<AudioStreamPlayerComponent>();
            ent["AudioStreamPlayer"]["FilePath"] = audio.AudioHandle->GetFilePath().string();
            ent["AudioStreamPlayer"]["Volume"] = audio.Volume;
        }

        doc.push_back(ent);

        for (const auto child : node.GetChildren()) {
            SerializeNode(child, doc);
        }
    }

    void SceneTree::OnAttach()
    {
        p_physicsWorld = new b2World({ 0.0f, -9.8f });
        auto view = m_Registry.view<RigidbodyComponent>();
        for (auto entityHandle : view) {
            Entity entity(entityHandle, this);
            auto& transform = entity.GetComponent<TransformComponent>();
            auto worldTransform = GetWorldSpaceTransform(entity);
            auto& rigidbody = entity.GetComponent<RigidbodyComponent>();

            b2BodyDef bodyDef;
            // TODO: conversion func
            bodyDef.type = (b2BodyType)rigidbody.Type;
            bodyDef.position.Set(worldTransform.Position.x, worldTransform.Position.y);
            bodyDef.angle = glm::radians(-worldTransform.Rotation);

            b2Body* body = p_physicsWorld->CreateBody(&bodyDef);
            body->SetFixedRotation(rigidbody.FreezeRotation);

            rigidbody.RuntimeBody = (void*)body;

            if (entity.HasComponent<BoxColliderComponent>()) {
                auto& boxCollider = entity.GetComponent<BoxColliderComponent>();

                b2PolygonShape polygonShape;
                polygonShape.SetAsBox(boxCollider.Size.x,
                                      boxCollider.Size.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &polygonShape;
                fixtureDef.density = boxCollider.Density;
                fixtureDef.friction = boxCollider.Friction;
                fixtureDef.restitution = boxCollider.Restitution;
                fixtureDef.restitutionThreshold = boxCollider.RestitutionThreshold;

                boxCollider.RuntimeFixture = (void*)body->CreateFixture(&fixtureDef);
            }
        }

        ScriptEngine::StartRuntime();

        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            script.ManagedScriptObject.InvokeMethod("OnStart");
        }
    }

    void SceneTree::OnDetach()
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            script.ManagedScriptObject.InvokeMethod("OnDestroy");
        }
        delete p_physicsWorld;
        p_physicsWorld = nullptr;

        Nigozi::ScriptEngine::EndRuntime();
    }

    void SceneTree::OnEvent(Event& event)
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            // script.ManagedScriptObject.InvokeMethod("")
        }
    }

    void SceneTree::OnUpdate(float timestep)
    {
        for (auto entity : m_destroyQueue) {
            if (!entity.HasComponent<UUIDComponent>()) {
                continue;
            }
            if (entity.HasComponent<ScriptComponent>()) {
                auto& script = entity.GetComponent<ScriptComponent>();
                script.ManagedScriptObject.InvokeMethod("OnDestroy");
            }
            entity.Destroy();
        }
        if (m_destroyQueue.size() > 0) {
            m_destroyQueue.clear();
        }

        m_Registry.sort<UUIDComponent>([&](const auto lhs, const auto rhs) {
            auto lhsEntity = m_entityMap.find(lhs.ID)->second;
            auto rhsEntity = m_entityMap.find(rhs.ID)->second;
            return lhsEntity.GetHandle() < rhsEntity.GetHandle();
            });
        
        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            script.ManagedScriptObject.InvokeMethod("OnUpdate", timestep);
        }

        auto audioView = m_Registry.view<AudioStreamPlayerComponent>();
        audioView.each([](auto audio) {
            audio.AudioHandle->Update();
            });

        const int32_t velocityIterations = 6;
        const int32_t positionIteration = 2;
        p_physicsWorld->Step(timestep, velocityIterations, positionIteration);

        auto view = m_Registry.view<RigidbodyComponent>();
        for (auto entityHandle : view) {
            Entity entity(entityHandle, this);
            auto& transform = entity.GetComponent<TransformComponent>();
            auto worldTransform = GetWorldSpaceTransform(entity);
            auto& rigidbody = entity.GetComponent<RigidbodyComponent>();

            b2Body* body = (b2Body*)rigidbody.RuntimeBody;
            const auto& position = body->GetPosition();

            glm::vec2 delta(
                position.x - worldTransform.Position.x,
                position.y - worldTransform.Position.y
            );
            float bodyAngle = glm::degrees(body->GetAngle());
            float rotationDelta = -(bodyAngle - (worldTransform.Rotation - transform.Rotation));

            transform.Position += delta;
            transform.Rotation = rotationDelta;
        }
    }

    void SceneTree::OnRender()
    {

        m_Registry.sort<SpriteRendererComponent>([](const SpriteRendererComponent& a, const SpriteRendererComponent& b) {
            return a.ZOrder < b.ZOrder;
            });

        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            script.ManagedScriptObject.InvokeMethod("OnRender");
        }
        
        auto cameraView = m_Registry.view<TransformComponent, CameraComponent>();
        cameraView.each([this](auto entity, auto& transform, auto& camera) {
            if (camera.Current) {
                OrthographicCamera orthoCamera(-camera.Aspect * camera.Zoom, camera.Aspect * camera.Zoom, -camera.Zoom, camera.Zoom);
                
                auto worldTransform = GetWorldSpaceTransform(Entity(entity, this));
                orthoCamera.SetPosition(glm::vec3(worldTransform.Position, 0.0f));
                orthoCamera.SetRotation(-worldTransform.Rotation);

                orthoCamera.SetMVPMatrix();
            }
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

    void SceneTree::OnImGuiRender()
    {
        auto scriptView = m_Registry.view<ScriptComponent>();
        for (auto entity : scriptView)
        {
            auto& script = m_Registry.get<ScriptComponent>(entity);
            script.ManagedScriptObject.InvokeMethod("OnImGuiRender");
        }
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

    Entity SceneTree::CreateEntity(const std::string& name, const std::string& tag, UUID uuid)
    {
        Entity entity = Entity(m_Registry.create(), this);
        auto& uuidComponent = entity.AddComponent<Nigozi::UUIDComponent>();
        uuidComponent.ID = uuid;

        if (m_entityMap.empty()) {
            entity.AddComponent<SceneComponent>();
            m_sceneRootUUID = uuidComponent.ID;
        }
        m_entityMap[uuidComponent.ID] = entity;

        auto& name_ = entity.AddComponent<NameComponent>();
        name_.Name = name.empty() ? "" : name;

        auto& tag_ = entity.AddComponent<TagComponent>();
        tag_.Tag = tag.empty() ? "" : tag;

        entity.AddComponent<TransformComponent>();
        entity.AddComponent<RelationshipComponent>();
        entity.AddComponent<NodeTypeComponent>();

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

    void SceneTree::QueueDestroyEntity(Entity entity)
    {
        m_destroyQueue.push_back(entity);
    }

    bool SceneTree::DestroyEntity(Entity entity)
    {
        entity.Destroy();
        return true;
    }

    CameraComponent* SceneTree::GetMainCamera()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto entityHandle : view) {
            auto& camera = Entity(entityHandle, this).GetComponent<CameraComponent>();
            if (camera.Current) {
                return &camera;
            }
        }
        return nullptr;
    }
}
