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
		WindowsWindow(const WindowProps& props, const EventCallbackFn& callback = nullptr);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Props.Width; }
		inline unsigned int GetHeight() const override { return m_Props.Height; }
		inline float GetAspectRatio() const override { return static_cast<float>(m_Props.Width) / static_cast<float>(m_Props.Height); }
		inline const WindowProps& GetWindowProps() const override { return m_Props; }

		inline WindowMode GetWindowMode() const override { return m_Props.Mode; }
		void SetWindowMode(WindowMode mode) override;
		
		inline const Ref<RenderContext>& GetContext() const override { return m_RenderContext; }
		inline void SetContext(const Ref<RenderContext>& context) override { m_RenderContext = context; };
		
		inline void* GetNativeWindow() const override { return m_Window; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline void SetUseImGUI(bool use) override { m_Props.UseImGUI = use; };
		inline bool UseImGUI() const override { return m_Props.UseImGUI; };
		bool IsKeyPressed(KeyCode code) const override;
		inline bool IsMouseButtonPressed(MouseCode button) const override { return m_MouseButtons.at(button); };

		inline bool IsMaximized() const override { return m_Props.Maximized; }
		inline bool IsMinimized() const override { return m_Props.Minimized; }
		void SetBorderless(bool borderless) override;
		void Close() override;
		inline void SetResizeable(bool resizable) override { m_Props.Resizeable = resizable;};
		inline bool IsResizeable() const override { return m_Props.Resizeable; };
		inline void KeepAspectRatio(bool keepAspect) override { m_Props.KeepAspect = keepAspect; }
		inline bool KeepsAspectRatio() const override { return m_Props.KeepAspect;}
		inline void LimitWindowSize(WindowLimits limits) override {};

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		DWORD GetWindowStyle(bool fullscreen, bool borderless, bool resizeable, bool maximized);
		DWORD GetWindowStyle();
		DWORD GetWindowStyleEx(bool fullscreen);
		DWORD GetWindowStyleEx();

		void ApplyAspectRatio(int edge, RECT* area);
		void UpdateWindowStyle();
		void UpdateWindowStyle(int widht, int height, int xpos = 0, int ypos = 0);
		void ClientToWindowRect(RECT* rect, DWORD style, BOOL iwas, DWORD exstyle) const;

		void FitToMonitor();
		void GetFullWindowSize(DWORD style, DWORD exStyle,
			int contentWidth, int contentHeight,
			int* fullWidth, int* fullHeight,
			UINT dpi) const;
		std::pair<float, float> GetContentScale() const;

		void ClipSize();

		void MarkFullscreen(bool fullscreen);
		void MarkFullscreen();

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		static WindowsWindow* s_MainWindow;

	private:
		HWND m_Window;
		HDC m_DeviceContextHandler;
		Ref<RenderContext> m_RenderContext;
		WNDCLASSEX m_Class;
		Ref<WindowsOS> m_OS;

		DWORD m_Style;
		DWORD m_StyleEx;

		WindowProps m_Props;

		struct SavedInfo
		{
			int		XPos, YPos;
			int		Width, Height;
		};

		SavedInfo m_SavedInfo{};

		WindowLimits m_Limits;
		
		std::unordered_map<KeyCode, bool> m_Keys;
		std::unordered_map<MouseCode, bool> m_MouseButtons;

		EventCallbackFn m_EventCallback;
	};
}