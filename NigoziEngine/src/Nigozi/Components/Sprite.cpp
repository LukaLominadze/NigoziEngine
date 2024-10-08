#include "ngpch.h"
#include "Sprite.h"
#include "Nigozi/Layers/LayerStack.h"
#include "Nigozi/Layers/RenderLayer.h"

namespace Nigozi
{
	Sprite::Sprite(const char*&& spritePath)
	{
		data.spritePath = spritePath;

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
		SDL_QueryTexture(texture, nullptr, nullptr, &data.dstrect.w, &data.dstrect.h);
	}

	IMPL_C_GET_INDEX_FUNC(Sprite, RenderLayer)
}
