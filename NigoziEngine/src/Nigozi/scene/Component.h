#pragma once

#include "ngpch.h"
#include "Nigozi/glcore/Texture.h"
#include "Nigozi/glcore/SubTexture.h"
#include "Script.h"
#include "Nigozi/audio/AudioEngine.h"
#include "Nigozi/glcore/OrthographicCamera.h"
#include "UUID.h"
#include "ref/Ref.h"
#include <Coral/HostInstance.hpp>
#include <Coral/GC.hpp>
#include <Coral/Array.hpp>
#include <Coral/Attribute.hpp>

namespace Nigozi {
	struct UUIDComponent {
		UUID ID;

		UUIDComponent() {}
		UUIDComponent(const UUIDComponent& other) = default;
		UUIDComponent(UUID uuid)
			:ID(uuid) {}
	};

	struct NodeTypeComponent {
		enum class Types
		{
			None = 0,
			Node,
			Scene,
			Camera,
			SpriteRenderer,
			Rigidbody,
			AudioStreamPlayer
		};

		Types Type = Types::None;
	};

	struct SceneComponent {
		std::filesystem::path FilePath;

		SceneComponent() {}
		SceneComponent(const SceneComponent& other) = default;
		SceneComponent(const std::filesystem::path& path)
			:FilePath(path) {}
	};

	struct NameComponent {
		std::string Name;

		NameComponent() {
			Name.reserve(128);
		}
		NameComponent(const NameComponent& other) = default;
		NameComponent(const std::string& name)
			:Name(name) {
			Name.reserve(128);
		}

		operator std::string& () { return Name; }
		operator const std::string& () const { return Name; }
	};

	struct TagComponent {
		std::string Tag{ "" };

		TagComponent() {
			Tag.reserve(128);
		}
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {
			Tag.reserve(128);
		}

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent {
		glm::vec2 Position{ 0.0f };
		glm::vec2 Scale{ 1.0f };
		float Rotation = 0.0f;

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(glm::vec2 position, glm::vec2 scale = glm::vec2(1.0f), float rotation = 0.0f)
			:Position(position), Scale(scale), Rotation(rotation) { }

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.0f), glm::vec3(Position, 0.0f))
								* glm::rotate(glm::mat4(1.0f), Rotation, glm::vec3(0.0f, 0.0f, 1.0f))
								* glm::scale(glm::mat4(1.0f), glm::vec3(Scale, 1.0f));
		}
	};

	struct RelationshipComponent {
		UUID ParentUUID = UUID(UUID::Null);
		std::vector<UUID> ChildrenUUIDs;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent& other) = default;
		RelationshipComponent(UUID parent)
			:ParentUUID(parent) {}
		RelationshipComponent(UUID parent, UUID child)
			:ParentUUID(parent) {
			ChildrenUUIDs.push_back(child);
		}
		RelationshipComponent(UUID parent, const std::vector<UUID>& children)
			:ParentUUID(parent), ChildrenUUIDs(children) {}
		RelationshipComponent(const std::vector<UUID>& children) {
			ChildrenUUIDs = children;
		}
	};

	struct CameraComponent {
		bool Current = true;
		float Aspect = 16.0f / 9.0f;
		float Zoom = 5.0f;

		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) = default;
		CameraComponent(bool current, float zoom)
			:Current(current), Zoom(zoom) { }
	};

	/*
		TODO: Having shared pointers everywhere kind of defeats the purpose
		of entt. Figure out a way to have these stack allocated
	*/ 
	struct SpriteRendererComponent {
		Ref<Texture> SpriteTexture;
		SubTexture Sprite;
		glm::vec4 Color{ 1.0f };
		int16_t ZOrder = 0;

		SpriteRendererComponent()
			:SpriteTexture(Renderer2D::GetData()->Textures[0]),
			 Sprite(SpriteTexture, SpriteTexture->GetSize()) { }
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const Ref<Texture> texture, const SubTexture& sprite, int8_t zOrder = 0) {
			SpriteTexture = texture;
			Sprite = sprite;
			ZOrder = zOrder;
		}
		SpriteRendererComponent(const std::string& filePath, glm::vec2 size, int8_t zOrder = 0, uint32_t slotX = 0, uint32_t slotY = 0)
			:SpriteTexture(Ref<Texture>::Create(filePath)),
			 Sprite(SpriteTexture, size, slotX, slotY),
			 ZOrder(zOrder) { }
	};

	// TODO: Maybe figure out a way to not have scripts heap allocated?
	struct ScriptComponent {
		std::filesystem::path ScriptPath;
		Coral::ManagedObject ManagedScriptObject;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent& other) = default;
	};

	struct RigidbodyComponent {
		// TODO:
		enum class BodyType 
		{
			Static = 0, Kinematic, Dynamic
		};

		BodyType Type = BodyType::Static;
		bool FreezeRotation = false;
		void* RuntimeBody = nullptr;

		RigidbodyComponent() = default;
		RigidbodyComponent(const RigidbodyComponent& other) = default;
	};

	struct BoxColliderComponent {
		glm::vec2 Size{ 0.5f, 0.5f };
		glm::vec2 Offset{ 0.0f, 0.0f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		void* RuntimeFixture = nullptr;

		BoxColliderComponent() = default;
		BoxColliderComponent(const BoxColliderComponent& other) = default;
	};

	struct AudioStreamPlayerComponent {
		float Volume = 0.0f;
		Audio* AudioHandle = nullptr;

		AudioStreamPlayerComponent() = default;
		AudioStreamPlayerComponent(const AudioStreamPlayerComponent& other) = default;
		AudioStreamPlayerComponent(const std::filesystem::path& filePath) {
			AudioHandle = AudioEngine::LoadAudioFromFile(filePath);
			ASSERT(AudioHandle, "Initializing audio...");
		}
		AudioStreamPlayerComponent(const std::filesystem::path& filePath, const std::string_view& audioGroup) {
			AudioHandle = AudioEngine::LoadAudioFromFile(filePath, audioGroup);
			ASSERT(AudioHandle, "Initializing audio...");
		}

		Audio* operator->() {
			return AudioHandle;
		}
	};
}