#include "ngpch.h"
#include "Application.h"

namespace Nigozi
{
	Application::Application(const char* title, int width, int height)
	{
		p_window = new Window(title, width, height);
		NG_Renderer = new Renderer(p_window->GetWindow());

		p_eventHandler = new EventHandler();

		p_objectLayer = new ObjectLayer();
		p_renderLayer = new RenderLayer(NG_Renderer);

		p_layerStack = new LayerStack();

		ASSERT(p_window, "Creating Window...");
		ASSERT(NG_Renderer, "Creating Renderer...");
		ASSERT(p_eventHandler, "Creating Events...");

		p_layerStack->PushLayer(p_objectLayer);
		p_layerStack->PushLayer(p_renderLayer);

		#ifdef _DEBUG
		for (Layer* layer : *p_layerStack) {
			ASSERT(layer, "Initializing Layers...");
		}
		#endif
	}

	Application::~Application()
	{
		delete &p_eventHandler->Get();

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

			p_eventHandler->PollEvents();
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
		if (p_eventHandler->GetWindowEvent(EventType::WindowClose)) {
			p_window->WindowClose();
		}
	}
}