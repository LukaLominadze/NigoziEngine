#pragma once
#include "ngpch.h"
#include "Nigozi/Core/Core.h"
#include "Nigozi/Core/Log.h"
#include "Nigozi/Core/Window.h"
#include "Nigozi/Components/Sprite.h"

namespace Nigozi
{
	class NG_API Renderer
	{
	public:
		Renderer(SDL_Window* p_window);
		~Renderer();

		void Clear();
		void DrawRect(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void DrawTexture(Sprite* p_sprite);
		void Present();

		SDL_Texture* CreateTexture(std::string spritePath);

		inline SDL_Renderer* GetRenderer() const { return p_renderer; }
	private:
		SDL_Renderer* p_renderer;
	};
}

