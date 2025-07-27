#pragma once

#include "ngpch.h"
#include "Nigozi/glcore/Texture.h"
#include "Nigozi/glcore/SubTexture.h"
#include "Script.h"

namespace Nigozi {
	struct NameComponent {
		std::string Name{ "" };

		NameComponent() = default;
		NameComponent(const NameComponent& other) = default;
		NameComponent(const std::string& name)
			:Name(name) {
		}

		operator std::string& () { return Name; }
		operator const std::string& () const { return Name; }
	};

	struct TagComponent {
		std::string Tag{ "" };

		TagComponent() = default;
		TagComponent(const TagComponent& other) = default;
		TagComponent(const std::string& tag)
			:Tag(tag) {
		}

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent {
		glm::vec3 Position{ 1.0f };
		glm::vec3 Scale{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(glm::vec3 position, glm::vec3 scale)
			:Position(position), Scale(1.0) { }
	};

	struct SpriteRendererComponent {
		std::shared_ptr<Texture> SpriteTexture;
		std::shared_ptr<SubTexture> Sprite;
		glm::vec4 Color{ 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& other) = default;
		SpriteRendererComponent(const std::shared_ptr<Texture>& texture, const std::shared_ptr<SubTexture> sprite) {
			SpriteTexture = texture;
			Sprite = sprite;
		}
		SpriteRendererComponent(const std::string& filePath, glm::vec2 size, uint32_t slotX = 0, uint32_t slotY = 0)
			:SpriteTexture(std::make_shared<Texture>(filePath)),
			 Sprite(std::make_shared<SubTexture>(SpriteTexture, size, slotX, slotY)) { }
	};

	struct ScriptComponent {
		std::shared_ptr<Script> ScriptHandle;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent& other) = default;
		ScriptComponent(const std::shared_ptr<Script>& script) {
			ScriptHandle = script;
		}
	};
}