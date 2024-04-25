#include "SandboxLayer.h"

SandboxLayer::SandboxLayer()
{
	m_object.AddComponent<Sprite>("Player.png");

	Sprite* p_sprite = m_object.GetComponent<Sprite>();

	p_sprite->data.srcrect = { 0, 0, 16, 16 };
	p_sprite->data.dstrect = { 150, 150, 300, 300 };
	p_sprite = nullptr;
}
