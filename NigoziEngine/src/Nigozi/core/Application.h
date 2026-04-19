#pragma once

#include "glcore/Shader.h"
#include "core/Window.h"
#include "layers/Layer.h"
#include "layers/LayerStack.h"
#include "layers/ImGuiLayer.h"
#include "Input.h"
#include "Benchmarking/Timer.h"
#include <queue>
#include <mutex>

namespace Nigozi 
{
	constexpr const char NO_ICON[5] = "NONE";

	struct ApplicationProps {
		const char* Title = "App";
		uint32_t Width = 960, Height = 540;
		const char* IconPath = NO_ICON; // No icon by default
		bool VSync = false;
		bool Fullscreen = false;
	};

	class Application {
	public:
		[[nodiscard]]
		explicit Application(const ApplicationProps& props);
		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		~Application();
		const bool Initialized() const { return m_initialized; }

		// Launch mainloop
		virtual void Run();
		static void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);
	protected:
		void QueueEvent(std::function<void(Event*)>&& func);
		virtual void OnEvent();
		virtual void OnUpdate(float timestep);
		virtual void OnRender();
		virtual void OnImGuiRender();
	private:
		bool SetupWindow(const ApplicationProps& props);
		bool CreateGUILayer();
		bool StartRenderer();
		bool StartAudioEngine();
	protected:
		ImGuiLayer m_imGuiLayer;
		LayerStack m_layerStack;
		Window m_window;
	private:
		std::mutex m_eventQueueMutex;
		std::queue<std::function<void(Event*)>> m_eventQueue;
		bool m_initialized = false;

		/*
			All events will be loaded in the same buffer
			on "OnEvent()" to reserve memory and keep it
			in the same place in the duration of the
			application
		*/
		char m_eventBuffer[16];
		Event* p_eventBufferPointer;
	};
}