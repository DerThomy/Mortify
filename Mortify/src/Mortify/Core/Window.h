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
		BorderlessWindow,
		Fullscreen
	};

	struct WindowLimits
	{
		std::optional<unsigned int> MinWidth;
		std::optional<unsigned int> MinHeight;
		std::optional<unsigned int> MaxWidth;
		std::optional<unsigned int> MaxHeight;

		WindowLimits(std::optional<unsigned int> minWidth = std::nullopt, std::optional<unsigned int> minHeight = std::nullopt,
			std::optional<unsigned int> maxWidth = std::nullopt, std::optional<unsigned int> maxHeight = std::nullopt)
			: MinWidth(minWidth), MinHeight(minHeight), MaxWidth(maxWidth), MaxHeight(maxHeight)
		{}
	};
	
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		WindowMode Mode;
		WindowLimits Limits;
		bool Maximized;
		bool Resizeable;
		bool KeepAspect;

		WindowProps(const std::string& title = "Mortify Engine", 
					unsigned int width = 1280, 
					unsigned int height = 720,
					WindowMode mode = WindowMode::BorderlessWindow,
					bool maximized = false,
					bool resizable = true,
					bool keepAspect = false,
					WindowLimits limits = WindowLimits())
			: Title(title), Width(width), Height(height), Mode(mode), Maximized(maximized),
			Resizeable(resizable), KeepAspect(keepAspect), Limits(limits) {}
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
		virtual float GetAspectRatio() const = 0;

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
		virtual bool IsMaximized() const = 0;
		virtual void Minimize() = 0;
		virtual bool IsMinimized() const = 0;
		virtual void Restore() = 0;
		virtual void Close() = 0;
		virtual void SetResizeable(bool resizable) = 0;
		virtual bool IsResizeable() const = 0;
		virtual void KeepAspectRatio(bool keepAspect) = 0;
		virtual bool KeepsAspectRatio() const = 0;
		virtual void LimitWindowSize(WindowLimits limits = WindowLimits()) = 0;
		

		static Scope<Window> Create(const WindowProps& props = WindowProps(), const EventCallbackFn& callback = nullptr);
	};
}