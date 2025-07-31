#include "Sandbox.h"
#include <Nigozi.h>

Sandbox::Sandbox(const Nigozi::ApplicationProps props)
	:Application(props),
	 m_cameraLayer(props.Width / (float)props.Height),
	 m_sandboxLayer(&m_cameraLayer)
{
	PushLayer(&m_cameraLayer);
	PushLayer(&m_sandboxLayer);
}
