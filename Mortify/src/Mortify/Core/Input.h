#pragma once

#include "Mortify/Core/Core.h"

#include "Mortify/Core/KeyCodes.h"
#include "Mortify/Core/MouseCodes.h"
#include "Mortify/Core/WindowManager.h"

namespace Mortify
{
	class Input
	{
	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(KeyCode key) { return IsKeyPressed(WindowManager::GetFocusedWindow(), key); }
		inline static bool IsMouseButtonClicked(MouseCode button) { return IsMouseButtonClicked(WindowManager::GetFocusedWindow(), button); }
		inline static std::pair<float, float> GetMousePosition() { return GetMousePosition(WindowManager::GetFocusedWindow()); }
		inline static float GetMouseX() { return GetMouseX(WindowManager::GetFocusedWindow()); }
		inline static float GetMouseY() { return GetMouseY(WindowManager::GetFocusedWindow()); }

		inline static bool IsKeyPressed(const Ref<Window>& window, KeyCode key) { return s_Instance->IsKeyPressedImpl(window, key); }
		inline static bool IsMouseButtonClicked(const Ref<Window>& window, MouseCode button) { return s_Instance->IsMouseButtonClickedImpl(window, button); }
		inline static std::pair<float, float> GetMousePosition(const Ref<Window>& window) { return s_Instance->GetMousePositionImpl(window); }
		inline static float GetMouseX(const Ref<Window>& window) { return s_Instance->GetMouseXImpl(window); }
		inline static float GetMouseY(const Ref<Window>& window) { return s_Instance->GetMouseYImpl(window); }

	protected:
		virtual bool IsKeyPressedImpl(const Ref<Window>& window, KeyCode key) = 0;
		virtual bool IsMouseButtonClickedImpl(const Ref<Window>& window, MouseCode button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl(const Ref<Window>& window) = 0;
		virtual float GetMouseXImpl(const Ref<Window>& window) = 0;
		virtual float GetMouseYImpl(const Ref<Window>& window) = 0;

	private:
		static Scope<Input> s_Instance;
	};
}