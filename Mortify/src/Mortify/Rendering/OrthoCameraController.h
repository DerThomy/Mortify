#pragma once

#include "Mortify/Rendering/Camera.h"
#include "Mortify/Core/Timestep.h"

#include "Mortify/Core/Events/MouseEvent.h"
#include "Mortify/Core/Events/ApplicationEvent.h"
#include "Mortify/Core/Events/WindowEvent.h"

namespace Mortify
{
	class OrthoCameraController
	{
	public:
		OrthoCameraController(bool rotation = false);
		OrthoCameraController(const Ref<Window>& window, bool rotation = false);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		OrthographicCamera m_Camera;
		Ref<Window> m_Window;
		
		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; //In degrees, in the anti-clockwise direction

		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 100.0f;
	};
}