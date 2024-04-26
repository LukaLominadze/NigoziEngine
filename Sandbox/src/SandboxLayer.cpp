#include "SandboxLayer.h"

using namespace Nigozi;

SandboxLayer::SandboxLayer()
{
	m_object.AddComponent<Sprite>("Player.png");

	m_object.GetComponent<Sprite>()->data.srcrect = { 0, 0, 16, 16 };
}

void SandboxLayer::OnUpdate()
{
	if (EventHandler::Get().GetKey(EventType::KeyDown, SDL_SCANCODE_D)) {
		m_object.m_Transform.Position.x += 0.03f;
	}
	if (EventHandler::Get().GetKey(EventType::KeyDown, SDL_SCANCODE_A)) {
		m_object.m_Transform.Position.x -= 0.03f;
	}
	if (EventHandler::Get().GetKey(EventType::KeyDown, SDL_SCANCODE_W)) {
		m_object.m_Transform.Position.y -= 0.03f;
	}
	if (EventHandler::Get().GetKey(EventType::KeyDown, SDL_SCANCODE_S)) {
		m_object.m_Transform.Position.y += 0.03f;
	}
}
