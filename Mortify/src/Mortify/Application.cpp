#include "mtpch.h"
#include "Application.h"

#include "Mortify/Events/ApplicationEvent.h"
#include "Mortify/Log.h"

#include <GLFW/glfw3.h>

namespace Mortify
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}