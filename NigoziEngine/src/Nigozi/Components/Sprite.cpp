#include "ngpch.h"
#include "Sprite.h"
#include "Nigozi/Layers/LayerStack.h"
#include "Nigozi/Layers/RenderLayer.h"

namespace Nigozi
{
	Sprite::Sprite()
	{
		SetTargetIndex();
		SEND_COMPONENT(Sprite, this);
	}

	Sprite::Sprite(const char*&& spritePath)
	{
		data.spritePath = spritePath;

		SetTargetIndex();
		SEND_COMPONENT(Sprite, this);

		spritePath = nullptr;
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::SetSprite(const char* spritePath)
	{
		data.spritePath = spritePath;
		SDL_DestroyTexture(data.p_texture);
	}

	void Sprite::SetTexture(SDL_Texture* texture)
	{
		data.p_texture = texture;
	}

	IMPL_TARGET_INDEX_FUNC(Sprite, RenderLayer)
}
