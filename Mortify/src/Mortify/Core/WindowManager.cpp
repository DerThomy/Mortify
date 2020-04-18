#include "mtpch.h"

#include "WindowManager.h"
#include "Window.h"

namespace Mortify
{
	std::unordered_map<WindowID, Ref<Window>> WindowManager::s_Windows{};
	uint16_t WindowManager::s_WindowCounter = 0;

	WindowID WindowManager::CreateWindowID()
	{
		return s_WindowCounter++;
	}

	Ref<Window> WindowManager::AddWindow(const WindowConfig& config, const EventCallbackFn& callback)
	{
		WindowID id = CreateWindowID();
		auto window = CreatePlatformWindow(id, config, callback);
		s_Windows[id] = window;
		return window;
	}

	void WindowManager::RemoveWindow(WindowID id)
	{
		if (s_Windows.find(id) != s_Windows.end())
		{
			s_Windows[id]->Shutdown();
			s_Windows.erase(id);
			s_WindowCounter--;
		}
	}

	std::optional<Ref<Window>> WindowManager::GetWindowByID(WindowID id)
	{
		if (s_Windows.find(id) == s_Windows.end())
			return std::nullopt;
		else
			return s_Windows[id];
	}
}