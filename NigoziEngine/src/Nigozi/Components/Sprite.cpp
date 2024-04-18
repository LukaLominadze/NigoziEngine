#include "ngpch.h"
#include "Sprite.h"
#include "Nigozi/Layers/LayerStack.h"
#include "Nigozi/Layers/RenderLayer.h"

namespace Nigozi
{
	Sprite::Sprite()
	{
		SetTargetIndex();
		SendComponent();
	}

	Sprite::Sprite(const char*&& spritePath)
	{
		data.spritePath = spritePath;

		SetTargetIndex();
		SendComponent();

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
