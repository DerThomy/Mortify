#include "mtpch.h"

#include "Window.h"
#include "Mortify/Core/WindowManager.h"

namespace Mortify
{
	Ref<Window> Window::Create(const WindowConfig& config, const EventCallbackFn& callback)
	{
		return WindowManager::AddWindow(config, callback);
	}
}