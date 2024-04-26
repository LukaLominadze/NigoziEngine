#include "SandboxLayer.h"

SandboxLayer::SandboxLayer()
{
	m_object.AddComponent<Sprite>("Player.png");

	Sprite* p_sprite = m_object.GetComponent<Sprite>();

	p_sprite->data.srcrect = { 0, 0, 16, 16 };
	p_sprite->data.dstrect = { 0, 200, 16, 16 };
	p_sprite = nullptr;
}

void SandboxLayer::OnUpdate()
{
	m_object.m_Transform.Position.x++;
	Camera::IncrOrthograpicSize(0.025f);
}
