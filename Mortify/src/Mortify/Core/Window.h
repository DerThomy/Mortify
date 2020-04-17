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
		Maximized,
		Minimized,
		Fullscreen
	};

	static std::unordered_map<WindowMode, std::string> windowModeToString
	{
		{WindowMode::Windowed, "Windowed"},
		{WindowMode::Maximized, "Maximized"},
		{WindowMode::Minimized, "Minimized"},
		{WindowMode::Fullscreen, "Fullscreen"},
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

	enum WindowFlag
	{
		Borderless		= BIT(0),
		DisableResize	= BIT(1),
		AlwaysOnTop		= BIT(2),
		AutoIconify		= BIT(3),
		ScaleToMonitor	= BIT(4),
		KeepAspectRatio	= BIT(5),
	};

	struct WindowConfig
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		WindowMode Mode;
		WindowLimits Limits;
		bool VSync;
		uint8_t Flags;

		WindowConfig(const std::string& title = "Mortify Engine",
			uint32_t width = 1280,
			uint32_t height = 720,
			WindowMode mode = WindowMode::Fullscreen,
			bool vsync = false,
			uint8_t flags = WindowFlag::Borderless | WindowFlag::ScaleToMonitor,
			WindowLimits limits = WindowLimits())
			: Title(title), Width(width), Height(height), Mode(mode)
			, VSync(vsync), Limits(limits), Flags(flags)
		{}
	};
	
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		WindowLimits Limits;
		bool Resizeable;
		bool KeepAspect;
		bool VSync;
		bool Borderless;
		bool AlwaysOnTop;
		bool AutoIconify;
		bool ScaleToMonitor;
		
		// Depend on WindowMode
		bool Maximized;
		bool Minimized;
		bool Fullscreen;

		WindowProps(const WindowConfig& config)
			: Title(config.Title), Width(config.Width), Height(config.Height), Limits(config.Limits), VSync(config.VSync),
			Resizeable(!(config.Flags & WindowFlag::DisableResize)), KeepAspect(config.Flags & WindowFlag::KeepAspectRatio),
			Borderless(config.Flags & WindowFlag::Borderless), AlwaysOnTop(config.Flags & WindowFlag::AlwaysOnTop),
			AutoIconify(config.Flags & WindowFlag::AutoIconify), ScaleToMonitor(config.Flags & WindowFlag::ScaleToMonitor),
			Fullscreen(config.Mode == WindowMode::Fullscreen), Maximized(config.Mode == WindowMode::Maximized),
			Minimized(config.Mode == WindowMode::Minimized) 
		{}
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
		virtual const WindowProps& GetWindowProps() const = 0;

		virtual WindowMode GetWindowMode() const = 0;
		virtual void SetWindowMode(WindowMode mode) = 0;
		
		virtual void* GetNativeWindow() const = 0;
		virtual const Ref<RenderContext>& GetContext() const = 0;
		virtual void SetContext(const Ref<RenderContext>& context) = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual bool IsKeyPressed(KeyCode code) const = 0;
		virtual bool IsMouseButtonPressed(MouseCode button) const = 0;
		virtual bool IsMaximized() const = 0;
		virtual bool IsMinimized() const = 0;
		virtual void SetFlags(uint8_t flags, bool state) = 0;
		virtual void Close() = 0;
		virtual void LimitWindowSize(WindowLimits limits = WindowLimits()) = 0;
		
		static Scope<Window> Create(const WindowConfig& props = WindowConfig(), const EventCallbackFn& callback = nullptr);
	};
}