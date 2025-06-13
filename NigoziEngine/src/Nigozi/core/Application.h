#pragma once

#include "glcore/Shader.h"
#include "core/Window.h"
#include "layers/Layer.h"
#include "layers/LayerStack.h"
#include "layers/ImGuiLayer.h"
#include "Input.h"
#include "Benchmarking/Timer.h"

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
		const bool Initialized() const { return initialized; }

		// Launch mainloop
		virtual void Run();
		static void Close();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);
	protected:
		virtual void OnEvent(Event& event);
		virtual void OnUpdate(float timestep);
		virtual void OnRender();
		virtual void OnImGuiRender();
	private:
		bool CreateWindow(const ApplicationProps& props);
		bool CreateGUILayer();
		bool StartRenderer();
		bool initialized = false;
	protected:
		Window* p_window;
		LayerStack m_layerStack;
		ImGuiLayer m_imGuiLayer;
	};
}