#include "ngpch.h"
#include "Nigozi/Core/Renderer.h"

namespace Nigozi 
{
	Renderer::Renderer(SDL_Window* p_window)
	{
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

	void Renderer::DrawTexture(Sprite* p_sprite)
	{
		SDL_RenderCopy(p_renderer, p_sprite->data.p_texture, &p_sprite->data.srcrect, &p_sprite->data.dstrect);
	}

	void Renderer::Present()
	{
		SDL_RenderPresent(p_renderer);
	}

	SDL_Texture* Renderer::CreateTexture(std::string spritePath)
	{
		SDL_Surface* p_surface = IMG_Load(spritePath.c_str());
		SDL_Texture* p_texture = SDL_CreateTextureFromSurface(p_renderer, p_surface);
		SDL_FreeSurface(p_surface);

		return p_texture;
	}
}