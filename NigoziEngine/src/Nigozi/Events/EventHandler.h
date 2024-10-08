#pragma once

#include "ngpch.h"

#include "Nigozi/Core/Core.h"

namespace Nigozi
{
	class NG_API EventHandler
	{
	public:
		EventHandler() = default;
		~EventHandler() = default;

		void PollEvents();

		bool GetWindowEvent(SDL_EventType type);

		bool KeyPressed(SDL_Scancode keycode);
		bool KeyDown(SDL_Scancode keycode);
		bool KeyReleased(SDL_Scancode keycode);
	private:
		SDL_Event m_event{};
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		SDL_Scancode m_lastkey = SDL_SCANCODE_UNKNOWN;
	};
}
