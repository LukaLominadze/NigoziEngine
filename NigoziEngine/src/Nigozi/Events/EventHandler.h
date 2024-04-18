#pragma once

#include "ngpch.h"

#include "Nigozi/Core/Core.h"
#include "Nigozi/Events/Event.h"

namespace Nigozi
{
	class NG_API EventHandler
	{
	public:
		EventHandler();
		~EventHandler();

		void PollEvents();

		bool GetWindowEvent(EventType type);
		bool GetKey(EventType type, SDL_Scancode keycode);

		inline static EventHandler& Get() { return *s_Instance; }
	private:
		SDL_Event m_event{};
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		static EventHandler* s_Instance;

		SDL_Scancode m_lastkey = SDL_SCANCODE_UNKNOWN;
	};
}
