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
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Width; }
		inline unsigned int GetHeight() const override { return m_Height; }

		inline WindowMode GetWindowMode() const override { return m_Mode; }
		void SetWindowMode(WindowMode mode) override;
		
		inline const Ref<RenderContext>& GetContext() const override { return m_RenderContext; }
		inline void SetContext(const Ref<RenderContext>& context) override { m_RenderContext = context; };
		
		inline void* GetNativeWindow() const override { return m_Window; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline void SetUseImGUI(bool use) override { m_UsesImGUI = use; };
		inline bool UseImGUI() const override { return m_UsesImGUI; };
		bool IsKeyPressed(KeyCode code) const override;
		inline bool IsMouseButtonPressed(MouseCode button) const override { return m_MouseButtons.at(button); };

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		void FitToMonitor();
		void getFullWindowSize(DWORD style, DWORD exStyle,
			int contentWidth, int contentHeight,
			int* fullWidth, int* fullHeight,
			UINT dpi);

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		HWND m_Window;
		HDC m_DeviceContextHandler;
		Ref<RenderContext> m_RenderContext;
		WNDCLASSEX m_Class;
		Ref<WindowsOS> m_OS;

		std::string m_Title;
		unsigned int m_Width, m_Height;
		WindowMode m_Mode;
		bool m_VSync;
		bool m_UsesImGUI;
		
		std::unordered_map<KeyCode, bool> m_Keys;
		std::unordered_map<MouseCode, bool> m_MouseButtons;

		EventCallbackFn m_EventCallback;
	};
}