#pragma once

#include "ngpch.h"
#include "Nigozi/glcore/Texture.h"
#include "Nigozi/glcore/SubTexture.h"
#include "Script.h"
#include "Nigozi/audio/AudioEngine.h"
#include "Nigozi/glcore/OrthographicCamera.h"

namespace Nigozi {
	struct NameComponent {
		std::string Name{ "" };

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
		std::shared_ptr<SubTexture> Sprite;
		glm::vec4 Color{ 1.0f };
		int16_t ZOrder = 0;

		SpriteRendererComponent()
			:SpriteTexture(std::make_shared<Texture>("src/Nigozi/res/textures/flatQuad.png")),
			 Sprite(std::make_shared<SubTexture>(SpriteTexture, SpriteTexture->GetSize())) { }
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const std::shared_ptr<Texture>& texture, const std::shared_ptr<SubTexture> sprite, int8_t zOrder = 0) {
			SpriteTexture = texture;
			Sprite = sprite;
			ZOrder = zOrder;
		}
		SpriteRendererComponent(const std::string& filePath, glm::vec2 size, int8_t zOrder = 0, uint32_t slotX = 0, uint32_t slotY = 0)
			:SpriteTexture(std::make_shared<Texture>(filePath)),
			 Sprite(std::make_shared<SubTexture>(SpriteTexture, size, slotX, slotY)),
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

	struct AudioStreamPlayerComponent {
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