#pragma once

#include "Mortify/Core/Window.h"
#include "Mortify/Rendering/RenderContext.h"

#include <GLFW/glfw3.h>
#include <Windows.h>

typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);

namespace Mortify
{
	class WindowsWindow;

	class WindowsGLContext : public Context
	{
	public:
		WindowsGLContext(HWND hwnd, HDC hdc);

		virtual void MakeContextCurrent() override;
		virtual procFunc GetProcFunc() override;
		virtual void SwapBuffers() override;
		virtual bool SetVsync(bool on) override;
		virtual void Destroy() override;

		static Context::procAdr getGLProcAddress(const char* procname);

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

	public:
		HWND m_WindowHandler;
		PIXELFORMATDESCRIPTOR m_PFD;
		HDC m_DeviceContextHandler;
		HGLRC m_OpenGLRenderContextHandler;
	};

	class WindowsWindow : public Window
	{
	private:
		friend class WindowsGLContext;
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline const Ref<Context>& WindowsWindow::GetContext() const override { return m_Context; }
		inline void SetContext(const Ref<Context>& context) override { m_Context = context; };
		
		inline virtual void* GetNativeWindow() const override { return m_Window; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline void SetUseImGUI(bool use) override { m_Data.UseImGUI = use; };
		inline bool UseImGUI() const override { return m_Data.UseImGUI; };
		inline bool IsKeyPressed(KeyCode code) const override 
		{ 
			if (code != KeyCode::Invalid) 
				return m_Data.m_Keys.at(code);
			else 
				MT_CORE_WARN("Invalid key"); 
			
			return false; 
		}
		inline bool IsMouseButtonPressed(MouseCode button) const override { return m_Data.m_MouseButtons.at(button); };

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		HWND m_Window;
		HDC m_DeviceContextHandler;
		Ref<Context> m_Context;
		Scope<RenderContext> m_RenderContext;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool UseImGUI;
			std::unordered_map<KeyCode, bool> m_Keys;
			std::unordered_map<MouseCode, bool> m_MouseButtons;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}