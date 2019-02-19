#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Mortify
{

	class MT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();
}

