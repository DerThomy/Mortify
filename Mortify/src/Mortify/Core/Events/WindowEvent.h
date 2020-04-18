#pragma once

#include "Mortify/Core/Events/Event.h"
#include "Mortify/Core/Window.h"

namespace Mortify
{
	class WindowEvent : public Event
	{
	public:
		inline WindowID GetWindowID() const { return m_WindowID; }

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	protected:
		WindowEvent(WindowID id)
			: m_WindowID(id)
		{}

		WindowID m_WindowID;
	};

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(WindowID id, unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height), WindowEvent(id) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream sstream;
			sstream << "WindowResizeEvent: " << m_Width << "(w), " << m_Height << "(h)";
			return sstream.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
	private:
		unsigned int m_Height, m_Width;
		WindowID m_WindowID;
	};

	class WindowCloseEvent : public WindowEvent
	{
	public:
		WindowCloseEvent(WindowID id)
			: WindowEvent(id)
		{}

		EVENT_CLASS_TYPE(WindowClose)
	};
}