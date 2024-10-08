#include "ngpch.h"
#include "Application.h"

namespace Nigozi
{
	Application::Application(const char* title, int width, int height)
	{
		p_window = new Window(title, width, height);
		NG_Renderer = new Renderer(p_window->GetWindow());

		m_eventHandler = EventHandler();
		p_input = new Input(&m_eventHandler);

		p_layerStack = new LayerStack();

		ASSERT(p_window, "Creating Window...");
		ASSERT(NG_Renderer, "Creating Renderer...");

		PushLayer(new ObjectLayer());
		PushLayer(new CollisionLayer());
		PushLayer(new RenderLayer(NG_Renderer));

		#ifdef _DEBUG
		for (Layer* layer : *p_layerStack) {
			ASSERT(layer, "Initializing Layers...");
		}
		#endif
	}

	Application::~Application()
	{
		delete &p_layerStack->Get();

		delete NG_Renderer;
		delete p_window;
	}

	void Application::PushLayer(Layer* layer)
	{
		p_layerStack->PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		p_layerStack->PopLayer(layer);
	}

	void Application::Run()
	{
		OnStart();

		while (p_window->Running()) {

			m_eventHandler.PollEvents();
			OnWindowEvent();

			OnUpdate();
			OnRender();
		}
	}

	void Application::OnStart()
	{
		std::cout << "Welcome to NIGOZI engine!" << std::endl;
	}

	void Application::OnUpdate()
	{
		for (Layer* layer : *p_layerStack) {
			layer->OnUpdate();
		}
	}

	void Application::OnRender()
	{
		NG_Renderer->Clear();

		for (Layer* layer : *p_layerStack) {
			layer->OnRender();
		}
		NG_Renderer->Present();

		SDL_Delay(1000 / 60);
	}

	void Application::OnWindowEvent()
	{
		if (m_eventHandler.GetWindowEvent(SDL_QUIT)) {
			p_window->WindowClose();
		}
	}
}