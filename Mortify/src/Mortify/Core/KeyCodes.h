#pragma once

#include "mtpch.h"

namespace Mortify
{
	typedef enum class KeyCode : uint16_t
	{
		Invalid = 0,
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

	inline KeyCode operator++(KeyCode& x) {
		return x = (KeyCode)(std::underlying_type<KeyCode>::type(x) + 1);
	}

	inline KeyCode operator*(KeyCode c) {
		return c;
	}

	inline KeyCode begin(KeyCode r) {
		return KeyCode::Space;
	}

	inline KeyCode end(KeyCode r) {
		KeyCode l = KeyCode::Menu;
		return ++l;
	}
}

// From glfw3.h
#define MT_KEY_FIRST           ::Mortify::Key::Space
#define MT_KEY_SPACE           ::Mortify::Key::Space
#define MT_KEY_APOSTROPHE      ::Mortify::Key::Apostrophe    /* ' */
#define MT_KEY_COMMA           ::Mortify::Key::Comma         /* , */
#define MT_KEY_MINUS           ::Mortify::Key::Minus         /* - */
#define MT_KEY_PERIOD          ::Mortify::Key::Period        /* . */
#define MT_KEY_SLASH           ::Mortify::Key::Slash         /* / */
#define MT_KEY_0               ::Mortify::Key::D0
#define MT_KEY_1               ::Mortify::Key::D1
#define MT_KEY_2               ::Mortify::Key::D2
#define MT_KEY_3               ::Mortify::Key::D3
#define MT_KEY_4               ::Mortify::Key::D4
#define MT_KEY_5               ::Mortify::Key::D5
#define MT_KEY_6               ::Mortify::Key::D6
#define MT_KEY_7               ::Mortify::Key::D7
#define MT_KEY_8               ::Mortify::Key::D8
#define MT_KEY_9               ::Mortify::Key::D9
#define MT_KEY_SEMICOLON       ::Mortify::Key::Semicolon     /* ; */
#define MT_KEY_EQUAL           ::Mortify::Key::Equal         /* = */
#define MT_KEY_A               ::Mortify::Key::A
#define MT_KEY_B               ::Mortify::Key::B
#define MT_KEY_C               ::Mortify::Key::C
#define MT_KEY_D               ::Mortify::Key::D
#define MT_KEY_E               ::Mortify::Key::E
#define MT_KEY_F               ::Mortify::Key::F
#define MT_KEY_G               ::Mortify::Key::G
#define MT_KEY_H               ::Mortify::Key::H
#define MT_KEY_I               ::Mortify::Key::I
#define MT_KEY_J               ::Mortify::Key::J
#define MT_KEY_K               ::Mortify::Key::K
#define MT_KEY_L               ::Mortify::Key::L
#define MT_KEY_M               ::Mortify::Key::M
#define MT_KEY_N               ::Mortify::Key::N
#define MT_KEY_O               ::Mortify::Key::O
#define MT_KEY_P               ::Mortify::Key::P
#define MT_KEY_Q               ::Mortify::Key::Q
#define MT_KEY_R               ::Mortify::Key::R
#define MT_KEY_S               ::Mortify::Key::S
#define MT_KEY_T               ::Mortify::Key::T
#define MT_KEY_U               ::Mortify::Key::U
#define MT_KEY_V               ::Mortify::Key::V
#define MT_KEY_W               ::Mortify::Key::W
#define MT_KEY_X               ::Mortify::Key::X
#define MT_KEY_Y               ::Mortify::Key::Y
#define MT_KEY_Z               ::Mortify::Key::Z
#define MT_KEY_LEFT_BRACKET    ::Mortify::Key::LeftBracket   /* [ */
#define MT_KEY_BACKSLASH       ::Mortify::Key::Backslash     /* \ */
#define MT_KEY_RIGHT_BRACKET   ::Mortify::Key::RightBracket  /* ] */
#define MT_KEY_GRAVE_ACCENT    ::Mortify::Key::GraveAccent   /* ` */
#define MT_KEY_WORLD_1         ::Mortify::Key::World1        /* non-US #1 */
#define MT_KEY_WORLD_2         ::Mortify::Key::World2        /* non-US #2 */

/* Function keys */
#define MT_KEY_ESCAPE          ::Mortify::Key::Escape
#define MT_KEY_ENTER           ::Mortify::Key::Enter
#define MT_KEY_TAB             ::Mortify::Key::Tab
#define MT_KEY_BACKSPACE       ::Mortify::Key::Backspace
#define MT_KEY_INSERT          ::Mortify::Key::Insert
#define MT_KEY_DELETE          ::Mortify::Key::Delete
#define MT_KEY_RIGHT           ::Mortify::Key::Right
#define MT_KEY_LEFT            ::Mortify::Key::Left
#define MT_KEY_DOWN            ::Mortify::Key::Down
#define MT_KEY_UP              ::Mortify::Key::Up
#define MT_KEY_PAGE_UP         ::Mortify::Key::PageUp
#define MT_KEY_PAGE_DOWN       ::Mortify::Key::PageDown
#define MT_KEY_HOME            ::Mortify::Key::Home
#define MT_KEY_END             ::Mortify::Key::End
#define MT_KEY_CAPS_LOCK       ::Mortify::Key::CapsLock
#define MT_KEY_SCROLL_LOCK     ::Mortify::Key::ScrollLock
#define MT_KEY_NUM_LOCK        ::Mortify::Key::NumLock
#define MT_KEY_PRINT_SCREEN    ::Mortify::Key::PrintScreen
#define MT_KEY_PAUSE           ::Mortify::Key::Pause
#define MT_KEY_F1              ::Mortify::Key::F1
#define MT_KEY_F2              ::Mortify::Key::F2
#define MT_KEY_F3              ::Mortify::Key::F3
#define MT_KEY_F4              ::Mortify::Key::F4
#define MT_KEY_F5              ::Mortify::Key::F5
#define MT_KEY_F6              ::Mortify::Key::F6
#define MT_KEY_F7              ::Mortify::Key::F7
#define MT_KEY_F8              ::Mortify::Key::F8
#define MT_KEY_F9              ::Mortify::Key::F9
#define MT_KEY_F10             ::Mortify::Key::F10
#define MT_KEY_F11             ::Mortify::Key::F11
#define MT_KEY_F12             ::Mortify::Key::F12
#define MT_KEY_F13             ::Mortify::Key::F13
#define MT_KEY_F14             ::Mortify::Key::F14
#define MT_KEY_F15             ::Mortify::Key::F15
#define MT_KEY_F16             ::Mortify::Key::F16
#define MT_KEY_F17             ::Mortify::Key::F17
#define MT_KEY_F18             ::Mortify::Key::F18
#define MT_KEY_F19             ::Mortify::Key::F19
#define MT_KEY_F20             ::Mortify::Key::F20
#define MT_KEY_F21             ::Mortify::Key::F21
#define MT_KEY_F22             ::Mortify::Key::F22
#define MT_KEY_F23             ::Mortify::Key::F23
#define MT_KEY_F24             ::Mortify::Key::F24
#define MT_KEY_F25             ::Mortify::Key::F25

/* Keypad */
#define MT_KEY_KP_0            ::Mortify::Key::KP0
#define MT_KEY_KP_1            ::Mortify::Key::KP1
#define MT_KEY_KP_2            ::Mortify::Key::KP2
#define MT_KEY_KP_3            ::Mortify::Key::KP3
#define MT_KEY_KP_4            ::Mortify::Key::KP4
#define MT_KEY_KP_5            ::Mortify::Key::KP5
#define MT_KEY_KP_6            ::Mortify::Key::KP6
#define MT_KEY_KP_7            ::Mortify::Key::KP7
#define MT_KEY_KP_8            ::Mortify::Key::KP8
#define MT_KEY_KP_9            ::Mortify::Key::KP9
#define MT_KEY_KP_DECIMAL      ::Mortify::Key::KPDecimal
#define MT_KEY_KP_DIVIDE       ::Mortify::Key::KPDivide
#define MT_KEY_KP_MULTIPLY     ::Mortify::Key::KPMultiply
#define MT_KEY_KP_SUBTRACT     ::Mortify::Key::KPSubtract
#define MT_KEY_KP_ADD          ::Mortify::Key::KPAdd
#define MT_KEY_KP_ENTER        ::Mortify::Key::KPEnter
#define MT_KEY_KP_EQUAL        ::Mortify::Key::KPEqual

#define MT_KEY_LEFT_SHIFT      ::Mortify::Key::LeftShift
#define MT_KEY_LEFT_CONTROL    ::Mortify::Key::LeftControl
#define MT_KEY_LEFT_ALT        ::Mortify::Key::LeftAlt
#define MT_KEY_LEFT_SUPER      ::Mortify::Key::LeftSuper
#define MT_KEY_RIGHT_SHIFT     ::Mortify::Key::RightShift
#define MT_KEY_RIGHT_CONTROL   ::Mortify::Key::RightControl
#define MT_KEY_RIGHT_ALT       ::Mortify::Key::RightAlt
#define MT_KEY_RIGHT_SUPER     ::Mortify::Key::RightSuper
#define MT_KEY_MENU            ::Mortify::Key::Menu

#define MT_LAST_KEY            MT_KEY_MENU

#define _MT_KEY_INVALID        ::Mortify::Key::Invalid

#ifdef MT_PLATFORM_WINDOWS
static std::unordered_map<int, ::Mortify::KeyCode> win32_keycodes = {
	{0x00B , MT_KEY_0},
	{0x002 , MT_KEY_1},
	{0x003 , MT_KEY_2},
	{0x004 , MT_KEY_3},
	{0x005 , MT_KEY_4},
	{0x006 , MT_KEY_5},
	{0x007 , MT_KEY_6},
	{0x008 , MT_KEY_7},
	{0x009 , MT_KEY_8},
	{0x00A , MT_KEY_9},
	{0x01E , MT_KEY_A},
	{0x030 , MT_KEY_B},
	{0x02E , MT_KEY_C},
	{0x020 , MT_KEY_D},
	{0x012 , MT_KEY_E},
	{0x021 , MT_KEY_F},
	{0x022 , MT_KEY_G},
	{0x023 , MT_KEY_H},
	{0x017 , MT_KEY_I},
	{0x024 , MT_KEY_J},
	{0x025 , MT_KEY_K},
	{0x026 , MT_KEY_L},
	{0x032 , MT_KEY_M},
	{0x031 , MT_KEY_N},
	{0x018 , MT_KEY_O},
	{0x019 , MT_KEY_P},
	{0x010 , MT_KEY_Q},
	{0x013 , MT_KEY_R},
	{0x01F , MT_KEY_S},
	{0x014 , MT_KEY_T},
	{0x016 , MT_KEY_U},
	{0x02F , MT_KEY_V},
	{0x011 , MT_KEY_W},
	{0x02D , MT_KEY_X},
	{0x015 , MT_KEY_Y},
	{0x02C , MT_KEY_Z},

	{0x028 , MT_KEY_APOSTROPHE },
	{0x02B , MT_KEY_BACKSLASH},
	{0x033 , MT_KEY_COMMA},
	{0x00D , MT_KEY_EQUAL},
	{0x029 , MT_KEY_GRAVE_ACCENT},
	{0x01A , MT_KEY_LEFT_BRACKET},
	{0x00C , MT_KEY_MINUS},
	{0x034 , MT_KEY_PERIOD},
	{0x01B , MT_KEY_RIGHT_BRACKET},
	{0x027 , MT_KEY_SEMICOLON},
	{0x035 , MT_KEY_SLASH},
	{0x056 , MT_KEY_WORLD_2},
	
	{0x00E , MT_KEY_BACKSPACE},
	{0x153 , MT_KEY_DELETE},
	{0x14F , MT_KEY_END},
	{0x01C , MT_KEY_ENTER},
	{0x001 , MT_KEY_ESCAPE},
	{0x147 , MT_KEY_HOME},
	{0x152 , MT_KEY_INSERT},
	{0x15D , MT_KEY_MENU},
	{0x151 , MT_KEY_PAGE_DOWN},
	{0x149 , MT_KEY_PAGE_UP},
	{0x045 , MT_KEY_PAUSE},
	{0x146 , MT_KEY_PAUSE},
	{0x039 , MT_KEY_SPACE},
	{0x00F , MT_KEY_TAB},
	{0x03A , MT_KEY_CAPS_LOCK},
	{0x145 , MT_KEY_NUM_LOCK},
	{0x046 , MT_KEY_SCROLL_LOCK},
	{0x03B , MT_KEY_F1},
	{0x03C , MT_KEY_F2},
	{0x03D , MT_KEY_F3},
	{0x03E , MT_KEY_F4},
	{0x03F , MT_KEY_F5},
	{0x040 , MT_KEY_F6},
	{0x041 , MT_KEY_F7},
	{0x042 , MT_KEY_F8},
	{0x043 , MT_KEY_F9},
	{0x044 , MT_KEY_F10},
	{0x057 , MT_KEY_F11},
	{0x058 , MT_KEY_F12},
	{0x064 , MT_KEY_F13},
	{0x065 , MT_KEY_F14},
	{0x066 , MT_KEY_F15},
	{0x067 , MT_KEY_F16},
	{0x068 , MT_KEY_F17},
	{0x069 , MT_KEY_F18},
	{0x06A , MT_KEY_F19},
	{0x06B , MT_KEY_F20},
	{0x06C , MT_KEY_F21},
	{0x06D , MT_KEY_F22},
	{0x06E , MT_KEY_F23},
	{0x076 , MT_KEY_F24},
	{0x038 , MT_KEY_LEFT_ALT},
	{0x01D , MT_KEY_LEFT_CONTROL},
	{0x02A , MT_KEY_LEFT_SHIFT},
	{0x15B , MT_KEY_LEFT_SUPER},
	{0x137 , MT_KEY_PRINT_SCREEN},
	{0x138 , MT_KEY_RIGHT_ALT},
	{0x11D , MT_KEY_RIGHT_CONTROL},
	{0x036 , MT_KEY_RIGHT_SHIFT},
	{0x15C , MT_KEY_RIGHT_SUPER},
	{0x150 , MT_KEY_DOWN},
	{0x14B , MT_KEY_LEFT},
	{0x14D , MT_KEY_RIGHT},
	{0x148 , MT_KEY_UP},
	
	{0x052 , MT_KEY_KP_0},
	{0x04F , MT_KEY_KP_1},
	{0x050 , MT_KEY_KP_2},
	{0x051 , MT_KEY_KP_3},
	{0x04B , MT_KEY_KP_4},
	{0x04C , MT_KEY_KP_5},
	{0x04D , MT_KEY_KP_6},
	{0x047 , MT_KEY_KP_7},
	{0x048 , MT_KEY_KP_8},
	{0x049 , MT_KEY_KP_9},
	{0x04E , MT_KEY_KP_ADD},
	{0x053 , MT_KEY_KP_DECIMAL},
	{0x135 , MT_KEY_KP_DIVIDE},
	{0x11C , MT_KEY_KP_ENTER},
	{0x059 , MT_KEY_KP_EQUAL},
	{0x037 , MT_KEY_KP_MULTIPLY},
	{0x04A , MT_KEY_KP_SUBTRACT}
};

static ::Mortify::KeyCode translateWin32Keys(WPARAM wparam, LPARAM lparam)
{
	// The Ctrl keys require special handling
	if (wparam == VK_CONTROL)
	{
		MSG next;
		DWORD time;

		// Right side keys have the extended key bit set
		if (lparam & 0x01000000)
			return MT_KEY_RIGHT_CONTROL;

		// HACK: Alt Gr sends Left Ctrl and then Right Alt in close sequence
		//       We only want the Right Alt message, so if the next message is
		//       Right Alt we ignore this (synthetic) Left Ctrl message
		time = GetMessageTime();

		if (PeekMessageW(&next, NULL, 0, 0, PM_NOREMOVE))
		{
			if (next.message == WM_KEYDOWN ||
				next.message == WM_SYSKEYDOWN ||
				next.message == WM_KEYUP ||
				next.message == WM_SYSKEYUP)
			{
				if (next.wParam == VK_MENU &&
					(next.lParam & 0x01000000) &&
					next.time == time)
				{
					// Next message is Right Alt down so discard this
					return _MT_KEY_INVALID;
				}
			}
		}

		return MT_KEY_LEFT_CONTROL;
	}

	if (wparam == VK_PROCESSKEY)
	{
		// IME notifies that keys have been filtered by setting the virtual
		// key-code to VK_PROCESSKEY
		return _MT_KEY_INVALID;
	}

	return win32_keycodes[HIWORD(lparam) & 0x1FF];
}
#endif MT_PLATFORM_WINDOWS