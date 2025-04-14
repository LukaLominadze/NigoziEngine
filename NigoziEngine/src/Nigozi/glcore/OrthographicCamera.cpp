#include "ngpch.h"

#include "OrthographicCamera.h"
#include "Renderer2D.h"
#include "core/Input.h"

namespace Nigozi
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_position(glm::vec3(0.0f)), m_rotation(0.0f)
	{
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewMatrix = glm::mat4(1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

		m_zoom = top;
		m_aspect = right / top;
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

		m_zoom = top;
		m_aspect = right / top;
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

	const glm::vec2 OrthographicCamera::GetMousePositionWorldSpace() const
	{
		std::pair<float, float> mousePosition = Input::GetMousePosition();
		mousePosition.second = Global::windowData.Height - mousePosition.second;
		float normalWidth = m_aspect;
		float transformedPositionX = (mousePosition.first / Global::windowData.Width) * normalWidth - 0.5f * normalWidth;
		float transformedPositionY = mousePosition.second / Global::windowData.Height - 0.5f;
		if (m_rotation != 0) {
			float radians = glm::radians(m_rotation);

			float cosTheta = cos(radians);
			float sinTheta = sin(radians);
			if (cosTheta == 0) {
				cosTheta = 1;
			}
			if (sinTheta == 0) {
				sinTheta = 1;
			}

			float rotatedX = cosTheta * transformedPositionX - sinTheta * transformedPositionY;
			float rotatedY = sinTheta * transformedPositionX + cosTheta * transformedPositionY;

			transformedPositionX = rotatedX;
			transformedPositionY = rotatedY;
		}
		return { transformedPositionX * 2 * m_zoom + m_position.x, transformedPositionY * 2 * m_zoom + m_position.y };
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation), glm::vec3(0, 0, 1));

		m_viewMatrix = glm::inverse(transform);
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
}
