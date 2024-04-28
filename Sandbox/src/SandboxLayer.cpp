#include "SandboxLayer.h"

using namespace Nigozi;

void SandboxLayer::OnAttach()
{
	m_object.AddComponent<Sprite>("Player.png");

	m_object.GetComponent<Sprite>()->data.srcrect = { 0, 0, 16, 16 };
}

void SandboxLayer::OnUpdate()
{
	if (Input::KeyDown(SDL_SCANCODE_D)) {
		m_object.m_Transform.Position.x += 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_A)) {
		m_object.m_Transform.Position.x -= 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_W)) {
		m_object.m_Transform.Position.y -= 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_S)) {
		m_object.m_Transform.Position.y += 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_K)) {
		Camera::IncrOrthograpicSize(0.1f);
	}
	if (Input::KeyDown(SDL_SCANCODE_L)) {
		Camera::IncrOrthograpicSize(-0.1f);
	}
}
