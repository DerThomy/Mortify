#include "mtpch.h"

#include "Application.h"
#include "Mortify/Events/ApplicationEvent.h"
#include "Mortify/Log.h"

namespace Mortify
{

	Application::Application()
	{
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		MT_TRACE(e);

		while (true);
	}

}