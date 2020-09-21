#pragma once

#include "Mortify/Core/Events/Event.h"
#include "Mortify/Core/WindowCore.h"

namespace Mortify
{
	class Window;

	class WindowFocusEvent;

	class WindowManager
	{
	public:

		static WindowID CreateWindowID();

		static Ref<Window> AddWindow(const WindowConfig& config = WindowConfig(), const EventCallbackFn& callback = nullptr);

		static void RemoveWindow(WindowID id, bool destroy = true);

		static std::vector<Ref<Window>> GetWindows();
		static std::optional<Ref<Window>> GetWindowByID(WindowID id);
		static const Ref<Window>& GetFocusedWindow() { return s_FocusedWindow; };
		static uint16_t GetWindowCount();

		static void OnEvent(Event& e);

	private:
		static std::unordered_map<WindowID, Ref<Window>> s_Windows;
		static uint16_t s_WindowCounter;
		static Ref<Window> s_FocusedWindow;

	private:
		static bool OnWindowFocusEvent(WindowFocusEvent& e);

		static Ref<Window> CreatePlatformWindow(WindowID id, const WindowConfig& props = WindowConfig(), const EventCallbackFn& callback = nullptr);
	};
}