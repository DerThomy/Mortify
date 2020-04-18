#include "mtpch.h"

#include "Platform/Windows/WindowsInput.h"
#include "Mortify/Core/Application.h"

#include <Windows.h>
#include <GLFW/glfw3.h>

namespace Mortify
{
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IsKeyPressedImpl(KeyCode keycode)
	{
		return Application::Get().GetWindow()->IsKeyPressed(keycode);
	}

	bool WindowsInput::IsMouseButtonClickedImpl(MouseCode button)
	{
		return Application::Get().GetWindow()->IsMouseButtonPressed(button);
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(*(HWND*)Application::Get().GetWindow()->GetNativeWindow(), &p);

		return { static_cast<float>(p.x), static_cast<float>(p.y) };
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto[x, y] = GetMousePositionImpl();
		return y;
	}
}