#pragma once

#include "mtpch.h"

namespace Mortify
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button_0 = 0,
		Button_1 = 1,
		Button_2 = 2,
		Button_3 = 3,
		Button_4 = 4,
		Button_5 = 5,
		Button_6 = 6,
		Button_7 = 7,

		ButtonLast = Button_7,
		ButtonLeft = Button_0,
		ButtonRight = Button_1,
		ButtonMiddle = Button_2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}

	inline MouseCode operator++(MouseCode& x) {
		return x = (MouseCode)(std::underlying_type<MouseCode>::type(x) + 1);
	}

	inline MouseCode operator*(MouseCode c) {
		return c;
	}

	inline MouseCode begin(MouseCode r) {
		return MouseCode::Button_0;
	}

	inline MouseCode end(MouseCode r) {
		MouseCode l = MouseCode::ButtonLast;
		return ++l;
	}
}

// From glfw3.h
#define MT_MOUSE_BUTTON_1         ::Mortify::MouseCode::Button_0
#define MT_MOUSE_BUTTON_2         ::Mortify::MouseCode::Button_1
#define MT_MOUSE_BUTTON_3         ::Mortify::MouseCode::Button_2
#define MT_MOUSE_BUTTON_4         ::Mortify::MouseCode::Button_3
#define MT_MOUSE_BUTTON_5         ::Mortify::MouseCode::Button_4
#define MT_MOUSE_BUTTON_6         ::Mortify::MouseCode::Button_5
#define MT_MOUSE_BUTTON_7         ::Mortify::MouseCode::Button_6
#define MT_MOUSE_BUTTON_8         ::Mortify::MouseCode::Button_7
#define MT_MOUSE_BUTTON_LAST      ::Mortify::MouseCode::ButtonLast
#define MT_MOUSE_BUTTON_LEFT      ::Mortify::MouseCode::ButtonLeft
#define MT_MOUSE_BUTTON_RIGHT     ::Mortify::MouseCode::ButtonRight
#define MT_MOUSE_BUTTON_MIDDLE    ::Mortify::MouseCode::ButtonMiddle
