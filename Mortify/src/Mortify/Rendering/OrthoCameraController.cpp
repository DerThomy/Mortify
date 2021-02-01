#include "mtpch.h"
#include "Mortify/Rendering/OrthoCameraController.h"

#include "Mortify/Core/KeyCodes.h"
#include "Mortify/Core/Input.h"
#include "Mortify/Core/WindowManager.h"
#include "Mortify/Core/Application.h"

namespace Mortify
{
	OrthoCameraController::OrthoCameraController(bool rotation)
		: m_AspectRatio(Application::Get().GetWindow()->GetAspectRatio()),
		  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		  m_Rotation(rotation), m_Window(Application::Get().GetWindow())
	{
	}

	OrthoCameraController::OrthoCameraController(const Ref<Window>& window, bool rotation)
		: m_AspectRatio(window->GetAspectRatio()),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation), m_Window(window)
	{
	}

	void OrthoCameraController::OnUpdate(Timestep ts)
	{
		MT_PROFILE_FUNCTION();
		
		if (Mortify::Input::IsKeyPressed(m_Window, MT_KEY_LEFT) || Mortify::Input::IsKeyPressed(m_Window, MT_KEY_A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
		}
		else if (Mortify::Input::IsKeyPressed(m_Window, MT_KEY_RIGHT) || Mortify::Input::IsKeyPressed(m_Window, MT_KEY_D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
		}

		if (Mortify::Input::IsKeyPressed(m_Window, MT_KEY_UP) || Mortify::Input::IsKeyPressed(m_Window, MT_KEY_W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
		}
		else if (Mortify::Input::IsKeyPressed(m_Window, MT_KEY_DOWN) || Mortify::Input::IsKeyPressed(m_Window, MT_KEY_S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * static_cast<float>(ts);
		}

		if (m_Rotation)
		{
			if (Mortify::Input::IsKeyPressed(m_Window, MT_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * static_cast<float>(ts);
			if (Mortify::Input::IsKeyPressed(m_Window, MT_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * static_cast<float>(ts);

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		MT_PROFILE_FUNCTION();
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(MT_BIND_EVENT_FN(OrthoCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(MT_BIND_EVENT_FN(OrthoCameraController::OnWindowResizeEvent));
	}

	bool OrthoCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		MT_PROFILE_FUNCTION();

		m_ZoomLevel += e.GetYOffset() * 0.15f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		return false;
	}

	bool OrthoCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		MT_PROFILE_FUNCTION();


		MT_CORE_INFO("Update Aspect ratio");
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		return false;
	}
}