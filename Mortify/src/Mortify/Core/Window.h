#pragma once

#include "mtpch.h"

#include "Mortify/Core/Core.h"
#include "Mortify/Core/Events/Event.h"
#include "Mortify/Core/Input.h"

namespace Mortify
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Mortify Engine", 
					unsigned int width = 1280, 
					unsigned int height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	class Context
	{
	public:
		typedef void (*procAdr) (void);
		typedef procAdr (*procFunc)(const char*);

		virtual void MakeContextCurrent() = 0;
		virtual procFunc GetProcFunc() = 0;
		virtual void SwapBuffers() = 0;
		virtual bool SetVsync(bool on) = 0;
		virtual void Destroy() = 0;
	};

	// Window Interface for desktop system windows
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		
		virtual void* GetNativeWindow() const = 0;
		virtual const Ref<Context>& GetContext() const = 0;
		virtual void SetContext(const Ref<Context>& context) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetUseImGUI(bool use) = 0;
		virtual bool UseImGUI() const = 0;
		virtual bool IsKeyPressed(KeyCode code) const = 0;
		virtual bool IsMouseButtonPressed(MouseCode button) const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}