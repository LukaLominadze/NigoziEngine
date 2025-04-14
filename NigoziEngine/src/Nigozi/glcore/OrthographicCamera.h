#pragma once

#include "ngpch.h"

namespace Nigozi
{
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera();

		void SetProjection(float left, float right, float bottom, float top);
		void SetMVPMatrix();

		void SetPosition(const glm::vec3& position);
		void SetRotation(float rotation);

		inline const glm::vec3& GetPosition() const { return m_position; }
		inline const float GetRotation() const { return m_rotation; }
		const glm::vec2 GetMousePositionWorldSpace() const;

		inline const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

		glm::vec3 m_position;
		float m_rotation;

		float m_aspect;
		float m_zoom;
	};
}
