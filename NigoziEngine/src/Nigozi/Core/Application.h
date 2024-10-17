#pragma once

#include "glcore/Renderer.h"
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

		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		void OnEvent(Event& event);
		void OnUpdate(float timestep);
		void OnRender();
		void OnImGuiRender();
	private:
		Window* p_window;
		Renderer m_renderer;
		Input* p_input = nullptr;

		LayerStack m_layerStack;

		ImGuiLayer m_imGuiLayer;

		bool m_running;
	};
}