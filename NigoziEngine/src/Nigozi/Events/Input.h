#pragma once

#include "ngpch.h"

#include "Nigozi/Events/EventHandler.h"

namespace Nigozi 
{
	class Input {
	public:
		Input(EventHandler* eventHandler);
		~Input();

		static bool KeyPressed(SDL_Scancode keycode) { return s_instance->KeyPressedImpl(keycode); }
		static bool KeyDown(SDL_Scancode keycode) { return s_instance->KeyDownImpl(keycode); }
		static bool KeyReleased(SDL_Scancode keycode) { return s_instance->KeyReleasedImpl(keycode); }

		inline static Input& Get() { return *s_instance; }
	private:
		virtual bool KeyPressedImpl(SDL_Scancode keycode) { return p_eventHandler->KeyPressed(keycode); }
		virtual bool KeyDownImpl(SDL_Scancode keycode) { return p_eventHandler->KeyDown(keycode); }
		virtual bool KeyReleasedImpl(SDL_Scancode keycode) { return p_eventHandler->KeyReleased(keycode); }

		static Input* s_instance;

		EventHandler* p_eventHandler;
	};
}