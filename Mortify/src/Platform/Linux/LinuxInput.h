#pragma once

#include "Mortify/Core/Input.h"

namespace Mortify
{
	class LinuxInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(const Ref<Window>& window, KeyCode keycode) override;
		virtual bool IsMouseButtonClickedImpl(const Ref<Window>& window, MouseCode button) override;
		virtual std::pair<float, float> GetMousePositionImpl(const Ref<Window>& window) override;
		virtual float GetMouseXImpl(const Ref<Window>& window) override;
		virtual float GetMouseYImpl(const Ref<Window>& window) override;
	};
}