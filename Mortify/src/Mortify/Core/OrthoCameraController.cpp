#include "mtpch.h"
#include "OrthoCameraController.h"

#include "Mortify/Core/KeyCodes.h"
#include "Mortify/Core/Input.h"

namespace Mortify
{
	OrthoCameraController::OrthoCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		  m_Rotation(rotation)
	{
	}

	void OrthoCameraController::OnUpdate(Timestep ts)
	{
		if (Mortify::Input::IsKeyPressed(MT_KEY_LEFT) || Mortify::Input::IsKeyPressed(MT_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Mortify::Input::IsKeyPressed(MT_KEY_RIGHT) || Mortify::Input::IsKeyPressed(MT_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Mortify::Input::IsKeyPressed(MT_KEY_UP) || Mortify::Input::IsKeyPressed(MT_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		else if (Mortify::Input::IsKeyPressed(MT_KEY_DOWN) || Mortify::Input::IsKeyPressed(MT_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		if (m_Rotation)
		{
			if (Mortify::Input::IsKeyPressed(MT_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Mortify::Input::IsKeyPressed(MT_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthoCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthoCameraController::OnWindowResizeEvent));
	}

	bool OrthoCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		m_ZoomLevel += e.GetYOffset() * 0.15f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthoCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}