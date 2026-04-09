#pragma once

#include "ngpch.h"
#include "Nigozi/glcore/Texture.h"
#include "Nigozi/glcore/SubTexture.h"
#include "Script.h"
#include "Nigozi/audio/AudioEngine.h"
#include "Nigozi/glcore/OrthographicCamera.h"
#include "UUID.h"

namespace Nigozi {
	struct UUIDComponent {
		UUID ID;

		UUIDComponent() {}
		UUIDComponent(const UUIDComponent& other) = default;
		UUIDComponent(UUID uuid)
			:ID(uuid) {}
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
		std::shared_ptr<Texture> SpriteTexture;
		SubTexture Sprite;
		glm::vec4 Color{ 1.0f };
		int16_t ZOrder = 0;

		SpriteRendererComponent()
			:SpriteTexture(Renderer2D::GetData()->Textures[0]),
			 Sprite(SpriteTexture, SpriteTexture->GetSize()) { }
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const std::shared_ptr<Texture>& texture, const SubTexture& sprite, int8_t zOrder = 0) {
			SpriteTexture = texture;
			Sprite = sprite;
			ZOrder = zOrder;
		}
		SpriteRendererComponent(const std::string& filePath, glm::vec2 size, int8_t zOrder = 0, uint32_t slotX = 0, uint32_t slotY = 0)
			:SpriteTexture(std::make_shared<Texture>(filePath)),
			 Sprite(SpriteTexture, size, slotX, slotY),
			 ZOrder(zOrder) { }
	};

	// TODO: Maybe figure out a way to not have scripts heap allocated?
	struct ScriptComponent {
		std::shared_ptr<Script> ScriptHandle;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent& other) = default;
		ScriptComponent(const std::shared_ptr<Script>& script) {
			ScriptHandle = script;
		}
	};

	struct RigidbodyComponent {
		// TODO:
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