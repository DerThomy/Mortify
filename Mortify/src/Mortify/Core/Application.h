#pragma once

#include "Core.h"

#include "Mortify/Core/Window.h"
#include "Mortify/Core/LayerStack.h"
#include "Mortify/Core/Events/Event.h"
#include "Mortify/Core/Events/ApplicationEvent.h"
#include "Mortify/Core/OS.h"

#include "Mortify/ImGui/ImGuiLayer.h"

namespace Mortify
{
	class  Application
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
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
	private:
		Mortify::Scope<Window> m_Window;
		Mortify::Ref<OS> m_OS;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;
		bool m_Minimized = false;
		double m_TimeFromLastFrame = 0.0;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

