#include "ngpch.h"
#include "SceneTree.h"
#include "Entity.h"
#include "Component.h"

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

    void SceneTree::SerializeScene(const std::filesystem::path& filePath)
    {
        if (filePath.empty()) {
            return;
        }

        Entity entity = TryGetEntityByUUID(m_sceneRootUUID);
        if (entity == Entity()) {
            return;
        }

        entity.GetComponent<SceneComponent>().FilePath = filePath;

        NG_CORE_LOG_INFO("[Scene Tree] Serializing scene at: {}", filePath.string());
        
        constexpr uint64_t SERIALIZER_VERSION = 0;

        nlohmann::json data;

        data["metadata"]["version"] = SERIALIZER_VERSION;
        data["nodes"] = nlohmann::json::array();

        SerializeNode(entity, data["nodes"]);

        std::ofstream file(filePath);

        // set indent to 4
        file << std::setw(4) << data << std::endl;
    }

    void SceneTree::DeserializeScene(const std::filesystem::path& filePath)
    {
        if (filePath.empty() || !std::filesystem::exists(filePath)) {
            return;
        }

        NG_CORE_LOG_INFO("[Scene Tree] Derializing scene from: {}", filePath.string());

        std::ifstream file(filePath);
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        std::string content = buffer.str();

        nlohmann::json data = nlohmann::json::parse(content);
        if (!data.contains("nodes")) {
            return;
        }

        for (auto& ent : data["nodes"]) {
            if (ent.contains("scene") && !m_entityMap.empty()) {
                std::filesystem::path scenePath(ent["scene"]["filePath"].get<std::string>());
                DeserializeScene(scenePath);
            }

            Entity entity = Entity(m_Registry.create(), this);

            auto& uuid = entity.AddComponent<Nigozi::UUIDComponent>();
            uuid.ID = UUID(ent["uuid"]["id"].get<uint64_t>());

            auto& name = entity.AddComponent<NameComponent>();
            name.Name = ent["name"]["name"].get<std::string>();

            auto& tag = entity.AddComponent<TagComponent>();
            tag.Tag = ent["tag"]["tag"].get<std::string>();

            if (ent.contains("scene")) {
                auto& scene = entity.AddComponent<SceneComponent>();
                if (ent["scene"].contains("filePath"))
                    scene.FilePath = ent["scene"]["filePath"].get<std::string>();
                if (m_entityMap.empty()) {
                    m_sceneRootUUID = uuid.ID;
                }
            }
            m_entityMap[uuid.ID] = entity;

            auto& transform = entity.AddComponent<TransformComponent>();
            transform.Position = glm::vec2(
                ent["transform"]["position"]["x"].get<float>(),
                ent["transform"]["position"]["y"].get<float>()
            );
            transform.Scale = glm::vec2(
                ent["transform"]["scale"]["x"].get<float>(),
                ent["transform"]["scale"]["y"].get<float>()
            );
            transform.Rotation = ent["transform"]["rotation"].get<float>();

            auto& relationship = entity.AddComponent<RelationshipComponent>();
            relationship.ParentUUID = ent["relationship"]["parentUuid"].get<uint64_t>();

            if (relationship.ParentUUID.GetUUID() != UUID::Null) {
                entity.SetParentUUID(relationship.ParentUUID);
            }

            if (ent.contains("camera")) {
                auto& camera = entity.AddComponent<CameraComponent>();
                if (ent["camera"].contains("current"))
                    camera.Current = ent["camera"]["current"].get<bool>();

                if (ent["camera"].contains("aspect"))
                    camera.Aspect = ent["camera"]["aspect"].get<float>();

                if (ent["camera"].contains("zoom"))
                    camera.Zoom = ent["camera"]["zoom"].get<float>();
            }

            if (ent.contains("sprite")) {
                auto& sprite = entity.AddComponent<SpriteRendererComponent>();

                std::string texturePath;
                if (ent["sprite"].contains("texture") && ent["sprite"]["texture"].contains("filePath")) {
                    texturePath = ent["sprite"]["texture"]["filePath"].get<std::string>();
                }
                if (std::filesystem::exists(std::filesystem::path(texturePath))) {
                    sprite.SpriteTexture = std::make_shared<Texture>(texturePath);
                }
                else {
                    sprite.SpriteTexture = Renderer2D::GetData()->Textures[0];
                    NG_CORE_LOG_ERROR("[SceneTree] Couldn't find texture with path: {}", texturePath);
                }

                glm::vec4 color;
                if (ent["sprite"].contains("color")) {
                    color.x = ent["sprite"]["color"]["r"].get<float>();
                    color.y = ent["sprite"]["color"]["g"].get<float>();
                    color.z = ent["sprite"]["color"]["b"].get<float>();
                    color.w = ent["sprite"]["color"]["a"].get<float>();
                }

                glm::u32vec2 seperator(1);
                if (ent["sprite"].contains("seperator")) {
                    seperator = glm::u32vec2(
                        ent["sprite"]["seperator"]["x"].get<int32_t>(),
                        ent["sprite"]["seperator"]["y"].get<int32_t>()
                    );
                }

                glm::vec2 textureSize = sprite.SpriteTexture->GetSize();
                glm::vec2 subTextureSize(
                    textureSize.x / seperator.x,
                    textureSize.y / seperator.y
                );

                glm::u32vec2 slot(0);
                if (ent["sprite"].contains("slot")) {
                    slot = glm::u32vec2(
                        ent["sprite"]["slot"]["x"].get<uint32_t>(),
                        ent["sprite"]["slot"]["y"].get<uint32_t>()
                    );
                }
                
                sprite.Sprite = SubTexture(sprite.SpriteTexture, subTextureSize, slot.x, slot.y);
                sprite.Color = color;
            }

            if (ent.contains("rigidbody")) {
                auto& rigidbody = entity.AddComponent<RigidbodyComponent>();

                if (ent["rigidbody"].contains("bodyType")) {
                    rigidbody.Type = (RigidbodyComponent::BodyType)ent["rigidbody"]["bodyType"].get<int32_t>();
                }
                if (ent["rigidbody"].contains("freezeRotation")) {
                    rigidbody.FreezeRotation = ent["rigidbody"]["freezeRotation"].get<bool>();
                }
            }

            if (ent.contains("boxCollider")) {
                auto& boxCollider = entity.AddComponent<BoxColliderComponent>();
                if (ent["boxCollider"].contains("size")) {
                    boxCollider.Size.x = ent["boxCollider"]["size"]["x"].get<float>();
                    boxCollider.Size.y = ent["boxCollider"]["size"]["y"].get<float>();
                }
                if (ent["boxCollider"].contains("density")) {
                    boxCollider.Density = ent["boxCollider"]["density"].get<float>();
                }
                if (ent["boxCollider"].contains("friction")) {
                    boxCollider.Friction = ent["boxCollider"]["friction"].get<float>();
                }
                if (ent["boxCollider"].contains("restitution")) {
                    boxCollider.Restitution = ent["boxCollider"]["restitution"].get<float>();
                }
                if (ent["boxCollider"].contains("restitutionThreshold")) {
                    boxCollider.RestitutionThreshold = ent["boxCollider"]["restitutionThreshold"].get<float>();
                }
            }

            if (ent.contains("audioStreamPlayer")) {
                auto& audio = entity.AddComponent<AudioStreamPlayerComponent>();

                std::filesystem::path audioPath;
                if (ent["audioStreamPlayer"].contains("filePath")) {
                    audioPath = ent["audioStreamPlayer"]["filePath"].get<std::string>();
                }
                if (std::filesystem::exists(audioPath)) {
                    audio.AudioHandle = AudioEngine::LoadAudioFromFile(audioPath);
                }
                else {
                    NG_CORE_LOG_ERROR("[Scene Tree] Couldn't find audio file with path: {}", audioPath.string());
                }

                if (ent["audioStreamPlayer"].contains("volume")) {
                    audio.Volume = ent["audioStreamPlayer"]["volume"].get<float>();
                }
            }
        }
    }

    void SceneTree::ClearSceneTree()
    {
        m_Registry.clear();
        m_entityMap.clear();
        m_sceneRootUUID = UUID(UUID::Null);
        NG_CORE_LOG_INFO("[Scene Tree] Cleared tree");
    }

    void SceneTree::SerializeNode(Entity node, nlohmann::json& doc) 
    {
        nlohmann::json ent;

        auto& uuid = node.GetComponent<UUIDComponent>();
        ent["uuid"]["id"] = uuid.ID.GetUUID();

        auto& name = node.GetComponent<NameComponent>();
        ent["name"]["name"] = name.Name;

        auto& tag = node.GetComponent<TagComponent>();
        ent["tag"]["tag"] = tag.Tag;

        if (node.HasComponent<SceneComponent>()) {
            auto& scene = node.GetComponent<SceneComponent>();
            ent["scene"]["filePath"] = std::filesystem::absolute(scene.FilePath).string();
            if (uuid.ID != m_sceneRootUUID) {
                // if we are using an external scene, stop serializing
                doc.push_back(ent);
                return;
            }
        }

        auto& transform = node.GetComponent<TransformComponent>();
        ent["transform"]["position"]["x"] = transform.Position.x;
        ent["transform"]["position"]["y"] = transform.Position.y;

        ent["transform"]["scale"]["x"] = transform.Scale.x;
        ent["transform"]["scale"]["y"] = transform.Scale.y;

        ent["transform"]["rotation"] = transform.Rotation;

        auto& relationship = node.GetComponent<RelationshipComponent>();

        // children data will not be saved
        // parent data is enough to load and save entities
        ent["relationship"]["parentUuid"] = relationship.ParentUUID.GetUUID();

        if (node.HasComponent<CameraComponent>()) {
            auto& camera = node.GetComponent<CameraComponent>();
            ent["camera"]["current"] = camera.Current;
            ent["camera"]["aspect"] = camera.Aspect;
            ent["camera"]["zoom"] = camera.Zoom;
        }

        if (node.HasComponent<SpriteRendererComponent>()) {
            auto& sprite = node.GetComponent<SpriteRendererComponent>();
            ent["sprite"]["texture"]["filePath"] = sprite.SpriteTexture->GetPath();
            
            ent["sprite"]["color"]["r"] = sprite.Color.x;
            ent["sprite"]["color"]["g"] = sprite.Color.y;
            ent["sprite"]["color"]["b"] = sprite.Color.z;
            ent["sprite"]["color"]["a"] = sprite.Color.w;

            ent["sprite"]["slot"]["x"] = sprite.Sprite.GetSlotX();
            ent["sprite"]["slot"]["y"] = sprite.Sprite.GetSlotY();

            glm::i32vec2 seperators = glm::i32vec2((glm::vec2)sprite.Sprite.GetTextureSize() / sprite.Sprite.GetSize());
            if (seperators.x == 0)
                seperators.x++;
            if (seperators.y == 0)
                seperators.y++;
            ent["sprite"]["seperator"]["x"] = (int32_t)(seperators.x);
            ent["sprite"]["seperator"]["y"] = (int32_t)(seperators.y);
        }

        if (node.HasComponent<RigidbodyComponent>()) {
            auto& rigidbody = node.GetComponent<RigidbodyComponent>();
            ent["rigidbody"]["bodyType"] = (int32_t)rigidbody.Type;
            ent["rigidbody"]["freezeRotation"] = rigidbody.FreezeRotation;
        }

        if (node.HasComponent<BoxColliderComponent>()) {
            auto& boxCollider = node.GetComponent<BoxColliderComponent>();
            ent["boxCollider"]["size"]["x"] = boxCollider.Size.x;
            ent["boxCollider"]["size"]["y"] = boxCollider.Size.y;

            ent["boxCollider"]["density"] = boxCollider.Density;
            ent["boxCollider"]["friction"] = boxCollider.Friction;
            ent["boxCollider"]["restitution"] = boxCollider.Restitution;
            ent["boxCollider"]["restitutionThreshold"] = boxCollider.RestitutionThreshold;
        }

        if (node.HasComponent<AudioStreamPlayerComponent>()) {
            auto& audio = node.GetComponent<AudioStreamPlayerComponent>();
            ent["audioStreamPlayer"]["filePath"] = audio.AudioHandle->GetFilePath();
            ent["audioStreamPlayer"]["volume"] = audio.Volume;
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
                polygonShape.SetAsBox(boxCollider.Size.x * worldTransform.Scale.x,
                                      boxCollider.Size.y * worldTransform.Scale.y);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &polygonShape;
                fixtureDef.density = boxCollider.Density;
                fixtureDef.friction = boxCollider.Friction;
                fixtureDef.restitution = boxCollider.Restitution;
                fixtureDef.restitutionThreshold = boxCollider.RestitutionThreshold;

                boxCollider.RuntimeFixture = (void*)body->CreateFixture(&fixtureDef);
            }
        }
    }

    void SceneTree::OnDetach()
    {
        delete p_physicsWorld;
        p_physicsWorld = nullptr;
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
            NG_CORE_LOG_INFO("[Scene Tree] body angle: ID: {}, {}", (uint64_t)entityHandle, bodyAngle);
            float rotationDelta = -(bodyAngle - (worldTransform.Rotation - transform.Rotation));

            transform.Position += delta;
            transform.Rotation = rotationDelta;
        }
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
