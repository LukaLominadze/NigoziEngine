#pragma once

#include "glcore/Renderer.h"
#include "glcore/Shader.h"
#include "core/Window.h"
#include "layers/Layer.h"
#include "layers/ImGuiLayer.h"
#include "Input.h"
#include "Benchmarking/Timer.h"

namespace Nigozi 
{
	class Application {
	public:
		Application(const char* title, uint32_t width, uint32_t height, bool vsync, bool fullscreen);
		~Application();

		void Run();

		void PushLayer(Layer* layer);
	private:
		void OnEvent(Event& event);
		void OnUpdate(float timestep);
		void OnRender();
		void OnImGuiRender();
	private:
		Window* p_window;
		Renderer m_renderer;
		Input* p_input = nullptr;

		std::vector<Layer*> m_layerStack;

		ImGuiLayer m_imGuiLayer;

		bool m_running;
	};
}