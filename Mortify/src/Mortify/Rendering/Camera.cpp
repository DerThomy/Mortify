#include "mtpch.h"

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
		m_ViewMatrix(1.0f), m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top)
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
}
