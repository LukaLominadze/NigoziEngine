#pragma once

#include "ngpch.h"

#include "Nigozi/Components/Component.h"
#include "Nigozi/Components/Transform.h"

namespace Nigozi
{
	struct NG_API SpriteData {
		SDL_Texture* p_texture = nullptr;

		SDL_Rect dstrect = { 0, 0, 0, 0 };
		SDL_Rect srcrect = { 0, 0, 0, 0 };

		std::string spritePath = "";
	};

	class NG_API Sprite : public Component
	{
	public:
		Sprite() = default;
		Sprite(const char*&& spritePath);
		~Sprite();

		void SetSprite(const char* spritePath);
		void SetTexture(SDL_Texture* texture);

		BUILD_C_GET_INDEX_FUNC

		static const char* GetType() {
			return "Sprite";
		}

		SpriteData data;
	};
}

