#pragma once

namespace Nigozi
{
	class Script
	{
	public:
		Script() = default;
		Script(Entity entityHandle) {
			m_entityHandle = entityHandle;
			OnAttach();
		}
		~Script() {
			OnDetach();
		}

		virtual void OnAttach() {}
		virtual void OnUpdate(float timestep) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void OnDetach() {}
	private:
		Entity m_entityHandle;
	};
}

