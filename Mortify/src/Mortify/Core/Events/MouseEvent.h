#pragma once

#include "Mortify/Core/Events/Event.h"

namespace Mortify
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream sstream;
			sstream << "Mouse moved: " << m_MouseX << "(x), " << m_MouseY << "(y)";
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX;
		float m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream sstream;
			sstream << "Mouse scrolled: " << m_XOffset << "(x), " << m_YOffset << "(y)";
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	private:
		float m_XOffset;
		float m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetButtonCode() const { return m_ButtonCode; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)
	protected:
		MouseButtonEvent(int buttonCode)
			: m_ButtonCode(buttonCode) {}

		int m_ButtonCode;
	};

	class MouseButtonClickedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonClickedEvent(int buttonCode)
			: MouseButtonEvent(buttonCode) {}

		std::string ToString() const override {
			std::stringstream sstream;
			sstream << "Mousebutton clicked: " << m_ButtonCode;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MouseButtonClicked)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int buttonCode)
			: MouseButtonEvent(buttonCode) {}

		std::string ToString() const override {
			std::stringstream sstream;
			sstream << "Mousebutton released: " << m_ButtonCode;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}