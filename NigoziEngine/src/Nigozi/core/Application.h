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
		void QueueEvent(std::function<Event*()>&& func);
		virtual void OnEvent();
		virtual void OnUpdate(float timestep);
		virtual void OnRender();
		virtual void OnImGuiRender();
	private:
		bool CreateWindow(const ApplicationProps& props);
		bool CreateGUILayer();
		bool StartRenderer();
	protected:
		Window* p_window;
		LayerStack m_layerStack;
		ImGuiLayer m_imGuiLayer;
	private:
		std::mutex m_eventQueueMutex;
		std::queue<std::function<Event* ()>> m_eventQueue;
		bool m_initialized = false;
	};
}