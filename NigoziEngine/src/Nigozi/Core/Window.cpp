#include "ngpch.h"
#include "Window.h"
#include "Nigozi/Core/Camera.h"

namespace Nigozi
{
	Window::Window(const char* title, int width, int height)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		ASSERT(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Initializing SDL...");

		p_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, NULL);
		ASSERT(p_window, "Creating Window...");

		Camera::WindowHeight = height;
		Camera::SetOrthograpicSize(5);

		m_running = true;
	}

	Window::~Window()
	{
		SDL_DestroyWindow(p_window);
		LOG("Destroying Window...");
	}

	void Window::OnWindowResize()
	{
	}

	void Window::WindowClose()
	{
		m_running = false;
	}

	bool Window::Running()
	{
		return m_running;
	}
}
