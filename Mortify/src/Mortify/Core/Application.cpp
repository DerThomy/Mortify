#include "mtpch.h"
#include "Application.h"

#include "Mortify/Core/Log.h"
#include "Mortify/Core/Input.h"
#include "Mortify/Rendering/Renderer.h"

#include <GLFW/glfw3.h>


namespace Mortify
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MT_PROFILE_FUNCTION();
		
		MT_CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_OS = OS::Create();

		m_Window = Window::Create();
		m_Window->SetEventCallback(MT_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		MT_PROFILE_FUNCTION();
		
		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		MT_PROFILE_FUNCTION();
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(MT_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(MT_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			(*it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()	
	{
		MT_PROFILE_FUNCTION();
		
		while (m_Running)
		{
			MT_PROFILE_SCOPE("RunLoop");
			
			float time = m_OS->GetTime();
			Timestep ts = time - m_TimeFromLastFrame;
			m_TimeFromLastFrame = time;

			if (!m_Minimized)
			{
				{
					MT_PROFILE_SCOPE("LayerStack OnUpdate");
					
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
				}
			}

			{
				MT_PROFILE_SCOPE("LayerStack OnImGuiRender");
				
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}
	
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		MT_PROFILE_FUNCTION();
		
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
			m_Minimized = true;
		else
			m_Minimized = false;
		
		return false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		MT_PROFILE_FUNCTION();
		
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		MT_PROFILE_FUNCTION();
		
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}