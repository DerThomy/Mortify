#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mortify/Events/Event.h"
#include "Mortify/Window.h"

namespace Mortify {

	class PerspectiveCamera
	{
	public:


	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		float m_Pitch;
		float m_Yaw;
		float m_Roll;
	};

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }

		inline float GetRotation() const { return m_Rotation; }
		inline void SetRotation(float rotation) { m_Rotation = rotation; UpdateViewMatrix(); }

		inline float GetAspectRatio() const { return m_AspectRatio; }
		inline void SetAspectRatio(float aspect)
		{
			m_AspectRatio = aspect;
			m_ProjectionMatrix = glm::ortho(m_Left * m_AspectRatio, m_Right * m_AspectRatio, m_Bottom, m_Top, -1.0f, 1.0f);
		}

		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void UpdateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_AspectRatio = 1.0f;

		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;
	};
}