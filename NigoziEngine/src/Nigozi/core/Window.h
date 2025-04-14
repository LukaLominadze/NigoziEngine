#pragma once

#include "events/Event.h"
#include "events/KeyEvent.h"

#include <functional>

namespace Nigozi
{
	struct WindowData {
		uint32_t Width, Height;
		bool Fullscreen = false;

		std::function<void(Event&)> EventCallback;
		std::function<void(bool)> SetFullscreen;

		GLFWwindow* NativeWindow;
	};

	namespace Global 
	{
		inline WindowData windowData;
	}

	class Window {
	public:
		Window() = default;
		Window(const char* title, uint32_t width, uint32_t height, bool fullscreen);
		~Window();

		static bool IsFullscreen();
		bool SetFullscreen(bool value);

		inline void SetEventCallback(const std::function<void(Event&)>& function) {
			Global::windowData.EventCallback = function;
		}

		void SetVSync(bool value);

		void OnUpdate();

		void Delete();
	private:
		GLFWwindow* p_window;
		GLFWmonitor* p_monitor;

		bool m_isFullscreen = false;
	};
}