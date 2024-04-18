#pragma once

#include "ngpch.h"

#include "Nigozi/Components/Component.h"

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
		Sprite();
		Sprite(const char*&& spritePath);
		~Sprite();

		void SetSprite(const char* spritePath);
		void SetTexture(SDL_Texture* texture);

		virtual void SetTargetIndex() override;

		virtual const std::type_info& GetType() const override {
			return typeid(*this);
		}

		SpriteData data;
	};
}

