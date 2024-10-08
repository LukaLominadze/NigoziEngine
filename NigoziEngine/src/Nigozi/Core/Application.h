#pragma once

#include "ngpch.h"

#include "Core.h"
#include "Nigozi/Core/Window.h"
#include "Nigozi/Core/Renderer.h"

#include "Nigozi/Events/EventHandler.h"
#include "Nigozi/Events/Input.h"

#include "Nigozi/Layers/Layer.h"
#include "Nigozi/Layers/LayerStack.h"
#include "Nigozi/Layers/ObjectLayer.h"
#include "Nigozi/Layers/CollisionLayer.h"
#include "Nigozi/Layers/RenderLayer.h"

#include "Nigozi/Components/Sprite.h"

namespace Nigozi {

	class NG_API Application
	{
	public:
		Application(const char* title, int width, int height);
		~Application();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void Run();
		void OnStart();
		void OnUpdate();
		void OnRender();

		void OnWindowEvent();

		Renderer* NG_Renderer;
	private:
		Window* p_window;

		LayerStack* p_layerStack;

		EventHandler m_eventHandler;
		Input* p_input;
	};
}

