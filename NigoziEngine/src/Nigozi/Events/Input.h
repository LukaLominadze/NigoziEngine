#pragma once

#include "ngpch.h"

#include "Nigozi/Events/EventHandler.h"

namespace Nigozi 
{
	class Input {
	public:
		Input(EventHandler* eventHandler);
		~Input();

		static bool KeyPressed(SDL_Scancode keycode) { return s_instance->p_eventHandler->KeyPressed(keycode); }
		static bool KeyDown(SDL_Scancode keycode) { return s_instance->p_eventHandler->KeyDown(keycode); }
		static bool KeyReleased(SDL_Scancode keycode) { return s_instance->p_eventHandler->KeyReleased(keycode); }

		inline static Input& Get() { return *s_instance; }
	private:
		static Input* s_instance;

		EventHandler* p_eventHandler;
	};
}