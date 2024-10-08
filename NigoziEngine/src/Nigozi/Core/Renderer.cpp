#include "ngpch.h"
#include "Nigozi/Core/Renderer.h"

namespace Nigozi 
{
	Renderer::Renderer(SDL_Window* p_window)
	{
		IMG_Init(IMG_INIT_PNG);

		p_renderer = SDL_CreateRenderer(p_window, 0, SDL_RENDERER_PRESENTVSYNC);
		ASSERT(p_renderer, "Creating Renderer...");
	}

	Renderer::~Renderer()
	{
		SDL_DestroyRenderer(p_renderer);
		LOG("Destroying Renderer...");
	}

	void Renderer::Clear()
	{
		SDL_SetRenderDrawColor(p_renderer, 30, 100, 50, 255);
		SDL_RenderClear(p_renderer);
	}

	void Renderer::DrawRect(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		SDL_SetRenderDrawColor(p_renderer, r, g, b, a);
		SDL_RenderFillRect(p_renderer, &rect);
	}

	void Renderer::DrawTexture(Sprite& sprite)
	{
		SDL_RenderCopy(p_renderer, sprite.data.p_texture, &sprite.data.srcrect, &sprite.data.dstrect);
	}

	void Renderer::Present()
	{
		SDL_RenderPresent(p_renderer);
	}

	void Renderer::CreateTexture(SpriteData& spriteData)
	{
		SDL_Surface* p_surface = IMG_Load(spriteData.spritePath.c_str());
		spriteData.p_texture = SDL_CreateTextureFromSurface(p_renderer, p_surface);

		int textureWidth, textureHeight;
		SDL_QueryTexture(spriteData.p_texture, nullptr, nullptr, &textureWidth, &textureHeight);
		LOG(textureWidth << ", " << textureHeight);

		if (spriteData.srcrect.w == 0) {
			spriteData.srcrect.w = p_surface->w;
			spriteData.srcrect.h = p_surface->h;
		}
		LOG("-> " << spriteData.dstrect.w);
		if (spriteData.dstrect.w == 0) {
			spriteData.dstrect.w = textureWidth;
			spriteData.dstrect.h = textureHeight;
		}

		SDL_FreeSurface(p_surface);
	}
}