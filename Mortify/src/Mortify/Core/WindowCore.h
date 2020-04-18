#pragma once

namespace Mortify
{
	typedef uint16_t WindowID;

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
		Borderless = BIT(0),
		DisableResize = BIT(1),
		AlwaysOnTop = BIT(2),
		AutoIconify = BIT(3),
		ScaleToMonitor = BIT(4),
		KeepAspectRatio = BIT(5),
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
			WindowMode mode = WindowMode::Windowed,
			bool vsync = false,
			uint8_t flags = 0,
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
			Resizeable(!(config.Flags& WindowFlag::DisableResize)), KeepAspect(config.Flags& WindowFlag::KeepAspectRatio),
			Borderless(config.Flags& WindowFlag::Borderless), AlwaysOnTop(config.Flags& WindowFlag::AlwaysOnTop),
			AutoIconify(config.Flags& WindowFlag::AutoIconify), ScaleToMonitor(config.Flags& WindowFlag::ScaleToMonitor),
			Fullscreen(config.Mode == WindowMode::Fullscreen), Maximized(config.Mode == WindowMode::Maximized),
			Minimized(config.Mode == WindowMode::Minimized)
		{}
	};
}