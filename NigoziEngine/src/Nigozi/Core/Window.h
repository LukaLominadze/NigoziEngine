#pragma once

#include "Nigozi/Core/Log.h"
#include <ngpch.h>

namespace Nigozi {

	class Window
	{
	public:
		Window(const char* title, int width, int height);
		~Window();

		void OnWindowResize();
		void WindowClose();

		bool Running();

		inline SDL_Window* GetWindow() const { return p_window; }

	private:
		SDL_Window* p_window;

		SDL_Event m_event;

		bool m_running;
	};
}

