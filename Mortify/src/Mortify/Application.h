#pragma once

#include "Core.h"

#include "Window.h"
#include "Mortify/LayerStack.h"
#include "Mortify/Events/Event.h"
#include "Mortify/Events/ApplicationEvent.h"

#include "Mortify/ImGui/ImGuiLayer.h"

namespace Mortify
{
	class MT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	
		inline Window& GetWindow() { return *m_Window; }
		
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

