#pragma once

#include "ngpch.h"

namespace Nigozi
{
	class PerspectiveCamera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);
		~PerspectiveCamera();

		void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);

		inline const glm::vec3& GetPosition() const { return m_position; }
		inline const glm::vec3& GetRotation() const { return m_rotation; }
		inline const glm::vec3& GetFront() const { return m_front; }
		inline const glm::vec3& GetRight() const { return m_right; }
		inline const glm::vec3& GetUp() const { return m_up; }

		inline float GetYaw() const { return m_yaw; }
		inline float GetPitch() const { return m_pitch; }

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
		glm::vec3 m_rotation;
		glm::vec3 m_front, m_right, m_up;
		float m_yaw = -90.0f, m_pitch = 0.0f;
		float m_fov, m_aspectRatio, m_nearClip, m_farClip;
	};
}
