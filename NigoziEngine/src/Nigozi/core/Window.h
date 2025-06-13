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
	};

	namespace Global 
	{
		inline WindowData windowData;
	}

	class Window {
	public:
		Window() = default;
		~Window();

		bool StartUp(const char* title, uint32_t width, uint32_t height, bool fullscreen);

		static bool IsFullscreen();
		static bool SetFullscreen(bool value);

		inline void SetEventCallback(const std::function<void(Event&)>& function) {
			Global::windowData.EventCallback = function;
		}

		void SetVSync(bool value);

		void OnUpdate();

		GLFWwindow* GetNativeWindow() const { return p_window; }

		void Delete();
	private:
		bool StartGLFW();
		bool CreateWindow(const char* title, uint32_t width, uint32_t height, bool fullscreen);
		bool StartGLEW();
		bool CreateCallbacks();
	private:
		GLFWwindow* p_window;
		GLFWmonitor* p_monitor;

		bool m_isFullscreen = false;
	};
}