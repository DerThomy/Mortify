#pragma once

#include "Event.h"

namespace Mortify
{
	class  KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput & EventCategoryKeyboard)
	protected:
		KeyEvent(int keyCode)
			: m_KeyCode(keyCode) {}

		int m_KeyCode;
	};

	class  KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		inline int getRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream sstream;
			sstream << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return sstream.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream sstream;
			sstream << "KeyReleasedEvent: " << m_KeyCode;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class  KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream sstream;
			sstream << "KeyTypedEvent: " << m_KeyCode;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}