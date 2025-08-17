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
	struct ApplicationProps {
		const char* Title;
		uint32_t Width, Height;
		bool VSync;
		bool Fullscreen;
		const char* IconPath;
	};

	class Application {
	public:
		Application(const ApplicationProps& props);
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
	protected:
		Window m_window;
		LayerStack m_layerStack;
		ImGuiLayer m_imGuiLayer;
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