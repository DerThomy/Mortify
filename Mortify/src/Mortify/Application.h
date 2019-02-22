#pragma once

#include "Core.h"

#include "Window.h"
#include "Mortify/LayerStack.h"
#include "Mortify/Events/Event.h"
#include "Mortify/Events/ApplicationEvent.h"

namespace Mortify
{

	class MT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);
		void Run();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_Layerstack;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

