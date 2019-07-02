#include "mtpch.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Mortify/KeyCodes.h"
#include "Mortify/MouseCodes.h"
#include "Mortify/Events/MouseEvent.h"
#include "Mortify/Events/KeyEvent.h"
#include "Mortify/Events/ApplicationEvent.h"


namespace Mortify
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		  m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::UpdateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& key_pressed_event = dynamic_cast<KeyPressedEvent&>(e);
			if (key_pressed_event.getKeyCode() == MT_KEY_W || key_pressed_event.getKeyCode() == MT_KEY_UP)
			{
				m_Position.y += 0.01f;
				UpdateViewMatrix();
			}
			else if (key_pressed_event.getKeyCode() == MT_KEY_A || key_pressed_event.getKeyCode() == MT_KEY_LEFT)
			{
				m_Position.x -= 0.01f;
				UpdateViewMatrix();
			}
			else if (key_pressed_event.getKeyCode() == MT_KEY_S || key_pressed_event.getKeyCode() == MT_KEY_DOWN)
			{
				m_Position.y -= 0.01f;
				UpdateViewMatrix();
			}
			else if (key_pressed_event.getKeyCode() == MT_KEY_D || key_pressed_event.getKeyCode() == MT_KEY_RIGHT)
			{
				m_Position.x += 0.01f;
				UpdateViewMatrix();
			}
			else if (key_pressed_event.getKeyCode() == MT_KEY_E)
			{
				m_Rotation += 1.0f;
				UpdateViewMatrix();
			}
			else if (key_pressed_event.getKeyCode() == MT_KEY_Q)
			{
				m_Rotation -= 1.0f;
				UpdateViewMatrix();
			}
		}
	}

	/*
	Camera::Camera() {
		m_CameraMode = FREE;
		m_CameraUp = glm::vec3(0, 1, 0);
		m_FOV = 45;
		m_CameraPositionDelta = glm::vec3(0, 0, 0);
		m_CameraScale = 0.5f;
		m_MaxPitchRate = 5;
		m_MaxHeadingRate = 5;
		m_MoveCamera = false;
		m_NearClip = 0.1;
		m_FarClip = 1000;
	}

	Camera::~Camera() {
	}

	void Camera::Reset() {
		m_CameraUp = glm::vec3(0, 1, 0);
	}

	void Camera::Update()
	{
		m_CameraDirection = glm::normalize(m_CameraLookAt - m_CameraPosition);

		if (m_CameraMode == CameraType::ORTHO)
		{
			m_Projection = glm::ortho(-1.5f * float(m_Aspect), 1.5f * float(m_Aspect), -1.5f, 1.5f, -10.0f, 10.0f);
		}
		else if (m_CameraMode == CameraType::FREE)
		{
			m_Projection = glm::perspective(m_FOV, m_Aspect, m_NearClip, m_FarClip);
			glm::vec3 axis = glm::cross(m_CameraDirection, m_CameraUp);
			glm::quat pitch_quat = glm::angleAxis(m_CameraPitch, axis);
			glm::quat heading_quat = glm::angleAxis(m_CameraHeading, m_CameraUp);
			glm::quat quat = glm::normalize(glm::cross(pitch_quat, heading_quat));

			m_CameraDirection = glm::rotate(quat, m_CameraDirection);

			m_CameraPosition += m_CameraPositionDelta;

			m_CameraLookAt = m_CameraPosition + m_CameraDirection * 1.0f;

			m_CameraHeading *= 0.5f;
			m_CameraPitch *= 0.5f;
			m_CameraPositionDelta = m_CameraPositionDelta * 0.8f;
		}

		m_View = glm::lookAt(m_CameraPosition, m_CameraLookAt, m_CameraUp);
		m_VP = m_Projection * m_View;
	}

	//Setting Functions
	void Camera::SetMode(CameraType cam_mode) {
		m_CameraMode = cam_mode;
		m_CameraUp = glm::vec3(0, 1, 0);
	}

	void Camera::SetPosition(glm::vec3 pos) {
		m_CameraPosition = pos;
	}

	void Camera::SetLookAt(glm::vec3 pos) {
		m_CameraLookAt = pos;
	}
	void Camera::SetFOV(double fov) {
		m_FOV = fov;
	}
	void Camera::SetViewport(int loc_x, int loc_y, int width, int height) {
		m_ViewportX = loc_x;
		m_ViewportY = loc_y;
		m_WindowWidth = width;
		m_WindowHeight = height;
		//need to use doubles division here, it will not work otherwise and it is possible to get a zero aspect ratio with integer rounding
		m_Aspect = double(width) / double(height);
	}
	void Camera::SetClipping(double near_clip_distance, double far_clip_distance) {
		m_NearClip = near_clip_distance;
		m_FarClip = far_clip_distance;
	}

	void Camera::Move(CameraDirection dir) {
		if (m_CameraMode == FREE) {
			switch (dir) {
				case UP:
					m_CameraPositionDelta += m_CameraUp * m_CameraScale;
					break;
				case DOWN:
					m_CameraPositionDelta -= m_CameraUp * m_CameraScale;
					break;
				case LEFT:
					m_CameraPositionDelta -= glm::cross(m_CameraDirection, m_CameraUp) * m_CameraScale;
					break;
				case RIGHT:
					m_CameraPositionDelta += glm::cross(m_CameraDirection, m_CameraUp) * m_CameraScale;
					break;
				case FORWARD:
					m_CameraPositionDelta += m_CameraDirection * m_CameraScale;
					break;
				case BACK:
					m_CameraPositionDelta -= m_CameraDirection * m_CameraScale;
					break;
			}
		}
	}

	void Camera::ChangePitch(float degrees) {
		//Check bounds with the max pitch rate so that we aren't moving too fast
		if (degrees < -m_MaxPitchRate) {
			degrees = -m_MaxPitchRate;
		}
		else if (degrees > m_MaxPitchRate) {
			degrees = m_MaxPitchRate;
		}
		m_CameraPitch += degrees;

		//Check bounds for the camera pitch
		if (m_CameraPitch > 360.0f) {
			m_CameraPitch -= 360.0f;
		}
		else if (m_CameraPitch < -360.0f) {
			m_CameraPitch += 360.0f;
		}
	}

	void Camera::ChangeHeading(float degrees) {
		//Check bounds with the max heading rate so that we aren't moving too fast
		if (degrees < -m_MaxHeadingRate) {
			degrees = -m_MaxHeadingRate;
		}
		else if (degrees > m_MaxHeadingRate) {
			degrees = m_MaxHeadingRate;
		}
		//This controls how the heading is changed if the camera is pointed straight up or down
		//The heading delta direction changes
		if (m_CameraPitch > 90 && m_CameraPitch < 270 || (m_CameraPitch < -90 && m_CameraPitch > -270)) {
			m_CameraHeading -= degrees;
		}
		else {
			m_CameraHeading += degrees;
		}
		//Check bounds for the camera heading
		if (m_CameraHeading > 360.0f) {
			m_CameraHeading -= 360.0f;
		}
		else if (m_CameraHeading < -360.0f) {
			m_CameraHeading += 360.0f;
		}
	}

	void Camera::Move2D(int x, int y) {
		//compute the mouse delta from the previous mouse position
		glm::vec3 mouse_delta = m_MousePosition - glm::vec3(x, y, 0);
		//if the camera is moving, meaning that the mouse was clicked and dragged, change the pitch and heading
		if (m_MoveCamera) {
			ChangeHeading(.08f * mouse_delta.x);
			ChangePitch(.08f * mouse_delta.y);
		}
		m_MousePosition = glm::vec3(x, y, 0);
	}

	void Camera::OnEvent(Event& e)
	{
		if (e.GetEventType() == EventType::MouseMoved)
		{
			MouseMovedEvent& mouse_moved_event = dynamic_cast<MouseMovedEvent&>(e);
			Move2D(mouse_moved_event.GetX(), mouse_moved_event.GetY());
		}
		else if (e.GetEventType() == EventType::MouseButtonClicked)
		{
			MouseButtonClickedEvent& mouse_clicked_event = dynamic_cast<MouseButtonClickedEvent&>(e);
			if (mouse_clicked_event.GetButtonCode() == MT_MOUSE_BUTTON_1)
				m_MoveCamera = true;
		}
		else if (e.GetEventType() == EventType::MouseButtonReleased)
		{
			MouseButtonReleasedEvent& mouse_released_event = dynamic_cast<MouseButtonReleasedEvent&>(e);
			if (mouse_released_event.GetButtonCode() == MT_MOUSE_BUTTON_1)
				m_MoveCamera = false;
		}
		else if (e.GetEventType() == EventType::MouseScrolled)
		{
			MouseScrolledEvent& mouse_scrolled_event = dynamic_cast<MouseScrolledEvent&>(e);
			if (mouse_scrolled_event.GetYOffset() > 0)
				m_CameraPositionDelta += m_CameraUp * 0.05f;
			else if (mouse_scrolled_event.GetYOffset() < 0)
				m_CameraPositionDelta -= m_CameraUp * 0.05f;
		}
		else if (e.GetEventType() == EventType::KeyPressed)
		{
			KeyPressedEvent& key_pressed_event = dynamic_cast<KeyPressedEvent&>(e);
			if (key_pressed_event.getKeyCode() == MT_KEY_W || key_pressed_event.getKeyCode() == MT_KEY_UP)
				Move(FORWARD);
			else if (key_pressed_event.getKeyCode() == MT_KEY_A || key_pressed_event.getKeyCode() == MT_KEY_LEFT)
				Move(LEFT);
			else if (key_pressed_event.getKeyCode() == MT_KEY_S || key_pressed_event.getKeyCode() == MT_KEY_DOWN)
				Move(BACK);
			else if (key_pressed_event.getKeyCode() == MT_KEY_D || key_pressed_event.getKeyCode() == MT_KEY_RIGHT)
				Move(RIGHT);
			else if (key_pressed_event.getKeyCode() == MT_KEY_Q)
				Move(DOWN);
			else if (key_pressed_event.getKeyCode() == MT_KEY_E)
				Move(UP);
		}
		else if (e.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent& window_resize_event = dynamic_cast<WindowResizeEvent&>(e);
			SetViewport(0, 0, window_resize_event.GetWidth(), window_resize_event.GetHeight());
		}
	}
	*/
}