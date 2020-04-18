#pragma once

#include "Mortify/Core/Events/Event.h"
#include "Mortify/Core/WindowCore.h"

namespace Mortify
{
	class Window;

	class WindowManager
	{
	public:

		static WindowID CreateWindowID();

		static Ref<Window> AddWindow(const WindowConfig& config = WindowConfig(), const EventCallbackFn& callback = nullptr);

		static void RemoveWindow(WindowID id);

		static std::optional<Ref<Window>> GetWindowByID(WindowID id);

		static uint16_t GetWindowCount() { return s_WindowCounter; }

	private:
		static std::unordered_map<WindowID, Ref<Window>> s_Windows;
		static uint16_t s_WindowCounter;

	private:
		static Ref<Window> CreatePlatformWindow(WindowID id, const WindowConfig& props = WindowConfig(), const EventCallbackFn& callback = nullptr);
	};
}