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
		glm::vec2 Position{ 0.0f, 0.0f };
		glm::vec2 Scale{ 1.0f, 1.0f };
		float Rotation = 0.0f;

		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) = default;
		TransformComponent(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f), float rotation = 0.0f)
			:Position(position), Scale(scale), Rotation(rotation) { }
	};

	/*
		TODO: Having shared pointers everywhere kind of defeats the purpose
		of entt. Figure out a way to have these stack allocated
	*/ 
	struct SpriteRendererComponent {
		std::shared_ptr<Texture> SpriteTexture;
		std::shared_ptr<SubTexture> Sprite;
		glm::vec4 Color{ 1.0f };
		int8_t ZOrder = 0;

		SpriteRendererComponent() = default;
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
}