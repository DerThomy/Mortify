#pragma once

#include "mtpch.h"

namespace Mortify
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
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
		return MouseCode::Button0;
	}

	inline MouseCode end(MouseCode r) {
		MouseCode l = MouseCode::ButtonLast;
		return ++l;
	}
}

// From glfw3.h
#define MT_MOUSE_BUTTON_1         ::Mortify::MouseCode::Button0
#define MT_MOUSE_BUTTON_2         ::Mortify::MouseCode::Button1
#define MT_MOUSE_BUTTON_3         ::Mortify::MouseCode::Button2
#define MT_MOUSE_BUTTON_4         ::Mortify::MouseCode::Button3
#define MT_MOUSE_BUTTON_5         ::Mortify::MouseCode::Button4
#define MT_MOUSE_BUTTON_6         ::Mortify::MouseCode::Button5
#define MT_MOUSE_BUTTON_7         ::Mortify::MouseCode::Button6
#define MT_MOUSE_BUTTON_8         ::Mortify::MouseCode::Button7
#define MT_MOUSE_BUTTON_LAST      ::Mortify::MouseCode::ButtonLast
#define MT_MOUSE_BUTTON_LEFT      ::Mortify::MouseCode::ButtonLeft
#define MT_MOUSE_BUTTON_RIGHT     ::Mortify::MouseCode::ButtonRight
#define MT_MOUSE_BUTTON_MIDDLE    ::Mortify::MouseCode::ButtonMiddle
