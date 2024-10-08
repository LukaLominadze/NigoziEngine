#include "SandboxLayer.h"

using namespace Nigozi;

void SandboxLayer::OnAttach()
{
	m_object1.m_Transform.Position = Vector2<float>(1, 2);
	m_object1.AddComponent<Sprite>("Player.png");
	m_object1.AddComponent<BoxCollider>();

	//m_object1.GetComponent<Sprite>()->data.srcrect = { 0, 0, 16, 16 };

	m_object2.m_Transform.Position = Vector2<float>(4, 4);
	m_object2.AddComponent<Sprite>("Square.png");
	m_object2.AddComponent<BoxCollider>();
	m_object1.GetComponent<BoxCollider>();
	std::cout << m_object1.GetComponent<Sprite>() << std::endl;

	//m_object2.GetComponent<Sprite>()->data.srcrect = { 0, 0, 16, 16 };
}

void SandboxLayer::OnUpdate()
{
	if (Input::KeyDown(SDL_SCANCODE_D)) {
		m_object1.m_Transform.Position.x += 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_A)) {
		m_object1.m_Transform.Position.x -= 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_W)) {
		m_object1.m_Transform.Position.y -= 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_S)) {
		m_object1.m_Transform.Position.y += 0.07f;
	}
	if (Input::KeyDown(SDL_SCANCODE_K)) {
		Camera::IncrOrthograpicSize(0.1f);
	}
	if (Input::KeyDown(SDL_SCANCODE_L)) {
		Camera::IncrOrthograpicSize(-0.1f);
	}
}

void SandboxLayer::OnRender()
{
}
