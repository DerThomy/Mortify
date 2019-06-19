#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Mortify {
	// Default camera values
	static const float YAW = -90.0f;
	static const float PITCH = 0.0f;
	static const float SPEED = 2.5f;
	static const float SENSITIVITY = 0.1f;
	static const float ZOOM = 45.0f;

	class Camera
	{
	public:
		enum Camera_Movement {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};

	public:
		Camera(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 up = { 0.0f, 1.0f, 0.0f }, float yaw = YAW, float pitch = PITCH)
			: m_Front(0.0f, 0.0f, -1.0f), m_Position(position), m_WorldUp(up), m_Yaw(yaw),
			m_Pitch(pitch), m_Speed(SPEED), m_Sensitivity(SENSITIVITY), m_Zoom(ZOOM)
		{
			updateCameraVectors();
		}

		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
			: m_Front(0.0f, 0.0f, -1.0f), m_Position(posX, posY, posZ), m_WorldUp(upX, upY, upZ),
			m_Pitch(pitch), m_Yaw(yaw), m_Speed(SPEED), m_Sensitivity(SENSITIVITY), m_Zoom(ZOOM)
		{
			updateCameraVectors();
		}

		inline glm::mat4 GetViewMatrix() const
		{
			return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}

		void ProcessKeyboard(Camera_Movement direction, float deltaTime);

		void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

		void ProcessMouseScroll(float yoffset);

	private:
		void updateCameraVectors();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		float m_Yaw;
		float m_Pitch;
		float m_Speed;
		float m_Sensitivity;
		float m_Zoom;
	};
}