#include "ngpch.h"

#include "OrthographicCamera.h"
#include "Renderer2D.h"

namespace Nigozi
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_position(glm::vec3(0.0f)), m_rotation(0.0f)
	{
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewMatrix = glm::mat4(1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::SetMVPMatrix()
	{
		Renderer2D::SetMVPMatrix(m_viewProjectionMatrix);
	}

	void OrthographicCamera::SetPosition(const glm::vec3& position)
	{
		m_position = position;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetRotation(float rotation)
	{
		m_rotation = rotation;
		RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}
