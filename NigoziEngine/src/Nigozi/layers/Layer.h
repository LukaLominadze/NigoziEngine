#pragma once

#include "events/Event.h"
#include "glcore/Renderer2D.h"

namespace Nigozi
{
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnEvent(Event& evnet) {}
		virtual void OnUpdate(float timestep) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnDetach() {}
	};
}
