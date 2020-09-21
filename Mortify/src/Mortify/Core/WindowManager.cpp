#include "mtpch.h"
#include "WindowManager.h"

#include "Mortify/Core/Application.h"
#include "Mortify/Core/Window.h"
#include "Mortify/Core/Events/WindowEvent.h"

namespace Mortify
{
	std::unordered_map<WindowID, Ref<Window>> WindowManager::s_Windows{};
	Ref<Window> WindowManager::s_FocusedWindow {};
	uint16_t WindowManager::s_WindowCounter = 0;

	WindowID WindowManager::CreateWindowID()
	{
        static uint16_t seed = 0;
		return seed++;
	}
    
    uint16_t WindowManager::GetWindowCount() { 
            return s_WindowCounter; 
    }

	Ref<Window> WindowManager::AddWindow(const WindowConfig& config, const EventCallbackFn& callback)
	{
		WindowID id = CreateWindowID();
		auto window = CreatePlatformWindow(id, config, callback);
		s_Windows[id] = window;

		if (!s_FocusedWindow)
			s_FocusedWindow = window;
            
        s_WindowCounter++;

		return window;
	}

	void WindowManager::RemoveWindow(WindowID id, bool destroy)
	{
		if (s_Windows.find(id) != s_Windows.end())
		{
            if (destroy) 
                s_Windows.at(id)->Close();
			s_Windows.erase(id);
			s_WindowCounter--;
		}
	}

	std::vector<Ref<Window>> WindowManager::GetWindows()
	{
		std::vector<Ref<Window>> windows;
		windows.reserve(s_Windows.size());

		for (auto window : s_Windows)
			windows.push_back(window.second);

		return windows;
	}

	std::optional<Ref<Window>> WindowManager::GetWindowByID(WindowID id)
	{
		if (s_Windows.find(id) == s_Windows.end())
			return std::nullopt;
		else
			return s_Windows[id];
	}

	bool WindowManager::OnWindowFocusEvent(WindowFocusEvent& e)
	{
		s_FocusedWindow = s_Windows[e.GetWindowID()];
		return false;
	}

	void WindowManager::OnEvent(Event& e)
	{
		auto dispatcher = EventDispatcher(e);
		dispatcher.Dispatch<WindowFocusEvent>(WindowManager::OnWindowFocusEvent);
	}
}