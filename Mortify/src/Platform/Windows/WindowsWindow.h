#pragma once

#include "Mortify/Core/Window.h"
#include "Mortify/Rendering/RenderContext.h"
#include "WindowsOS.h"

#include <GLFW/glfw3.h>
#include <Windows.h>

namespace Mortify
{
	class WindowsWindow : public Window
	{
	private:
		friend class WindowsGLRenderContext;
		friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	public:
		WindowsWindow(WindowID id, const WindowConfig& config, const EventCallbackFn& callback = nullptr);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Props.Width; }
		inline unsigned int GetHeight() const override { return m_Props.Height; }
		inline float GetAspectRatio() const override { return static_cast<float>(m_Props.Width) / static_cast<float>(m_Props.Height); }
		inline WindowID GetID() const override { return m_ID; }
		inline const WindowLimits& GetLimits() const { return m_Limits; }
		inline const std::string& GetTitle() const override { return m_Props.Title; }

		inline bool IsResizeable() const { return m_Props.Resizeable; }
		inline bool KeepsAspectRatio() const { return m_Props.KeepAspect; }
		inline bool IsBorderless() const { return m_Props.Borderless; }
		inline bool IsAlwaysOnTop() const { return m_Props.AlwaysOnTop; }
		inline bool AutoIconifys() const { return m_Props.AutoIconify; }
		inline bool ScalesToMonitor() const { return m_Props.ScaleToMonitor; }
		inline bool IsMaximized() const override { return IsZoomed(m_WindowHandle); }
		inline bool IsMinimized() const override { return IsIconic(m_WindowHandle); }
		inline bool IsFullscreen() const override { return m_Props.Fullscreen; }
		inline bool VSyncEnabled() const override { return m_Props.VSync; }
		bool IsKeyPressed(KeyCode code) const override;
		inline bool IsMouseButtonPressed(MouseCode button) const override { return m_MouseButtons.at(button); };

		inline WindowMode GetWindowMode() const override;
		void SetWindowMode(WindowMode mode) override;
		
		inline const Ref<RenderContext>& GetContext() const override { return m_RenderContext; }
		inline void SetContext(const Ref<RenderContext>& context) override { m_RenderContext = context; };
		
		inline void* GetNativeWindow() const override { return m_WindowHandle; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }
		void SetVSync(bool enabled) override;

		void SetFlags(uint8_t flags, bool state) override;
		void Close() override;
		inline void LimitWindowSize(WindowLimits limits) override { m_Props.Limits = limits; };

	private:
		void Init();
		void Shutdown() override;

		DWORD GetWindowStyle(bool fullscreen, bool borderless, bool resizeable, bool maximized);
		DWORD GetWindowStyle();
		DWORD GetWindowStyleEx(bool fullscreen);
		DWORD GetWindowStyleEx();

		void ApplyAspectRatio(uint32_t edge, RECT* area);
		void UpdateWindowStyle();
		void UpdateWindowStyle(uint32_t widht, uint32_t height, uint32_t xpos = 0, uint32_t ypos = 0);
		void ClientToWindowRect(RECT* rect, DWORD style, BOOL iwas, DWORD exstyle) const;

		void FitToMonitor();
		std::pair<uint32_t, uint32_t> GetFullWindowSize(
			DWORD style, DWORD exStyle, uint32_t contentWidth, uint32_t contentHeight, UINT dpi
		) const;
		std::pair<float, float> GetContentScale() const;

		void ClipSize();

		void MarkFullscreen(bool fullscreen);
		void MarkFullscreen();

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		static WindowsWindow* s_MainWindow;

	private:
		WindowID m_ID;
		HWND m_WindowHandle;
		HDC m_DeviceContextHandle;
		WNDCLASSEX m_WindowClass;
		Ref<RenderContext> m_RenderContext;
		Ref<WindowsOS> m_OS;

		WindowProps m_Props;
		WindowLimits m_Limits;

		EventCallbackFn m_EventCallback;

		uint32_t Width;
		uint32_t Height;

		bool Resizeable;
		bool KeepAspect;
		bool VSync;
		bool UseImGUI;
		bool Maximized;
		bool Minimized;
		bool Fullscreen;
		bool Borderless;

		struct SavedInfo
		{
			int XPos, YPos;
			int Width, Height;
		};
		SavedInfo m_SavedInfo{};
		
		std::unordered_map<KeyCode, bool> m_Keys;
		std::unordered_map<MouseCode, bool> m_MouseButtons;
	};
}