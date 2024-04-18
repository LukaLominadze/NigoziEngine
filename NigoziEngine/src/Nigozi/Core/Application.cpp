#include "ngpch.h"
#include "Application.h"

namespace Nigozi
{
	Application::Application(const char* title, int width, int height)
	{
		p_window = new Window(title, width, height);
		NG_Renderer = new Renderer(p_window->GetWindow());

		p_eventHandler = new EventHandler();

		p_renderLayer = new RenderLayer(NG_Renderer);
		p_objectLayer = new ObjectLayer();

		p_layerStack = new LayerStack();

		ASSERT(p_window, "Creating Window...");
		ASSERT(NG_Renderer, "Creating Renderer...");
		ASSERT(p_eventHandler, "Creating Events...");

		p_layerStack->PushLayer(p_renderLayer);
		p_layerStack->PushLayer(p_objectLayer);

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

	void Application::Run()
	{
		OnStart();

		while (p_window->Running()) {

			p_eventHandler->PollEvents();
			OnWindowEvent();

			OnUpdate();
			for (Layer* layer : *p_layerStack) {
				layer->OnUpdate();
			}

			NG_Renderer->Clear();
			OnRender();
			for (Sprite* sprite : p_renderLayer->GetSprites()) {
				NG_Renderer->DrawTexture(sprite);
			}
			NG_Renderer->Present();
		}
	}

	void Application::OnStart()
	{
		std::cout << "Welcome to NIGOZI engine!" << std::endl;
	}

	void Application::OnUpdate()
	{
	}

	void Application::OnRender()
	{
	}

	void Application::OnWindowEvent()
	{
		if (p_eventHandler->GetWindowEvent(EventType::WindowClose)) {
			p_window->WindowClose();
		}
	}
}