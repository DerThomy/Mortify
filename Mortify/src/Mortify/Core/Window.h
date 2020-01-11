#pragma once

#include "mtpch.h"

#include "Mortify/Core/Core.h"
#include "Mortify/Core/Events/Event.h"
#include "Mortify/Core/Input.h"
#include "Mortify/Rendering/RenderContext.h"

namespace Mortify
{
	enum class WindowMode
	{
		Windowed,
		Borderless,
		Fullscreen
	};

	struct WindowLimits
	{
		int MinWidth;
		int MinHeight;
		int MaxWidth;
		int MaxHeight;

		WindowLimits(int minWidht = MT_DONT_CARE, int minHeight = MT_DONT_CARE,
			int maxWidth = MT_DONT_CARE, int maxHeight = MT_DONT_CARE)
			: MinWidth(minWidht), MinHeight(minHeight), MaxWidth(maxWidth), MaxHeight(maxHeight)
		{}
	};
	
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowMode Mode;
		bool Maximized;
		bool Resizeable;
		bool KeepAspect;

		WindowProps(const std::string& title = "Mortify Engine", 
					unsigned int width = 1280, 
					unsigned int height = 720,
					WindowMode mode = WindowMode::Windowed,
					bool maximized = false,
					bool resizable = true,
					bool keepAspect = false)
			: Title(title), Width(width), Height(height), Mode(mode), Maximized(maximized),
			Resizeable(resizable), KeepAspect(keepAspect) {}
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

		virtual WindowMode GetWindowMode() const = 0;
		virtual void SetWindowMode(WindowMode mode) = 0;
		
		virtual void* GetNativeWindow() const = 0;
		virtual const Ref<RenderContext>& GetContext() const = 0;
		virtual void SetContext(const Ref<RenderContext>& context) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetUseImGUI(bool use) = 0;
		virtual bool UseImGUI() const = 0;
		virtual bool IsKeyPressed(KeyCode code) const = 0;
		virtual bool IsMouseButtonPressed(MouseCode button) const = 0;
		virtual void Maximize() = 0;
		virtual void SetResizeable(bool resizable) = 0;
		virtual bool IsResizeable() const = 0;
		virtual void SetKeepAspectRatio(bool keepAspect) = 0;
		virtual bool KeepAspectRatio() const = 0;
		virtual void LimitWindowSize(WindowLimits limits = WindowLimits()) = 0;
		

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}