#include "mtpch.h"

#include "Platform/Windows/WindowsInput.h"
#include "Mortify/Core/Application.h"
#include "Mortify/Core/Window.h"

#include <Windows.h>
#include <GLFW/glfw3.h>

namespace Mortify
{
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(const Ref<Window>& window, KeyCode keycode)
	{
		if (!window)
			return false;

		return window->IsKeyPressed(keycode);
	}

	bool WindowsInput::IsMouseButtonClickedImpl(const Ref<Window>& window, MouseCode button)
	{
		if (!window)
			return false;

		return window->IsMouseButtonPressed(button);
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl(const Ref<Window>& window)
	{
		if (!window)
			return { 0.0f, 0.0f };

		POINT p;
		GetCursorPos(&p);
		ScreenToClient(*(HWND*)window->GetNativeWindow(), &p);

		return { static_cast<float>(p.x), static_cast<float>(p.y) };
	}

	float WindowsInput::GetMouseXImpl(const Ref<Window>& window)
	{
		auto[x, y] = GetMousePositionImpl(window);
		return x;
	}

	float WindowsInput::GetMouseYImpl(const Ref<Window>& window)
	{
		auto[x, y] = GetMousePositionImpl(window);
		return y;
	}
}