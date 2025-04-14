#include "ngpch.h"

#include "PerspectiveCamera.h"

namespace Nigozi
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		:m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip),
		m_position(0.0f), m_rotation(0.0f)
	{
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
		RecalculateViewMatrix();
	}

	PerspectiveCamera::~PerspectiveCamera()
	{
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_fov = fov;
		m_aspectRatio = aspectRatio;
		m_nearClip = nearClip;
		m_farClip = farClip;
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position)
	{
		m_position = position;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		m_yaw = m_rotation.y;
		m_pitch = m_rotation.x;
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_front = glm::normalize(front);

		m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Cross product to get right vector
		m_up = glm::normalize(glm::cross(m_right, m_front));

		m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}
