#pragma once

#include "events/Event.h"
#include "events/KeyEvent.h"

#include <functional>

namespace Nigozi
{
	struct WindowData {
		const char* Title;
		uint32_t Width, Height;
		uint32_t WindowedWidth, WindowedHeight;
		bool Fullscreen, VSync;
		bool ShouldClose;

		std::function<void(std::function<void(Event*)>&&)> EventQueueCallback;
	};

	class Window
	{
	public:
		Window(const char* title, uint32_t width, uint32_t height, bool fullscreen = false, bool vsync = false);
		~Window();

		inline const bool Initialized() const { return m_initialized; }

		void SetIcon(const char* path);

		inline void SetEventCallback(const std::function<void(std::function<void(Event*)>&&)>& callback) {
			m_windowData.EventQueueCallback = callback;
		}

		void PollEvents();
		void Update();

		static void Close();

		void SetVSync(bool value);
		inline const bool IsVSync() const { return m_windowData.VSync; }

		void SetFullscreen(bool value);
		inline const bool IsFullscreen() const { return m_windowData.Fullscreen; }

		inline const bool ShouldClose() const { return m_windowData.ShouldClose; }

		inline WindowData& GetWindowData() { return m_windowData; }
	public:
		static void SetGlobalVSync(bool value);
		inline static bool IsGlobalVSync() { return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())).VSync; }

		static void SetGlobalFullscreen(bool value);
		inline static bool IsGlobalFullscreen() { return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())).Fullscreen; }

		inline static WindowData& GetGlobalWindowData() { return (*(WindowData*)glfwGetWindowUserPointer(glfwGetCurrentContext())); }
	private:
		bool StartGLFW();
		bool CreateWindow();
		bool SetupMonitor();
		bool StartGLEW();
		void CreateCallbacks();
	private:
		GLFWwindow* p_window = nullptr;
		GLFWmonitor* p_monitor = nullptr;

		WindowData m_windowData;

		bool m_initialized = false;
	};
}