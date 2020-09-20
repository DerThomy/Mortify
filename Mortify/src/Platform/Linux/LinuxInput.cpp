#include "mtpch.h"

#include "Platform/Linux/LinuxInput.h"
#include "Mortify/Core/Application.h"
#include "Mortify/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Mortify
{
	Scope<Input> Input::s_Instance = CreateScope<LinuxInput>();

	bool LinuxInput::IsKeyPressedImpl(const Ref<Window>& window, KeyCode keycode)
	{
		if (!window)
			return false;

		return window->IsKeyPressed(keycode);
	}

	bool LinuxInput::IsMouseButtonClickedImpl(const Ref<Window>& window, MouseCode button)
	{
		if (!window)
			return false;

		return window->IsMouseButtonPressed(button);
	}

	std::pair<float, float> LinuxInput::GetMousePositionImpl(const Ref<Window>& window)
	{
		if (!window)
			return { 0.0f, 0.0f };

        double x,y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window->GetNativeWindow()), &x, &y);

		return { static_cast<float>(x), static_cast<float>(y) };
	}

	float LinuxInput::GetMouseXImpl(const Ref<Window>& window)
	{
		auto[x, y] = GetMousePositionImpl(window);
		return x;
	}

	float LinuxInput::GetMouseYImpl(const Ref<Window>& window)
	{
		auto[x, y] = GetMousePositionImpl(window);
		return y;
	}
}