#pragma once

#include "Mortify/Core/Window.h"
#include "WindowsRenderContext.h"

#include <GLFW/glfw3.h>
#include <Windows.h>

namespace Mortify
{
	class WindowsWindow : public Window
	{
	private:
		friend class WindowsGLRenderContext;
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline const Ref<RenderContext>& WindowsWindow::GetContext() const override { return m_RenderContext; }
		inline void SetContext(const Ref<RenderContext>& context) override { m_RenderContext = context; };
		
		inline virtual void* GetNativeWindow() const override { return m_Window; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline void SetUseImGUI(bool use) override { m_Data.UseImGUI = use; };
		inline bool UseImGUI() const override { return m_Data.UseImGUI; };
		inline bool IsKeyPressed(KeyCode code) const override;
		inline bool IsMouseButtonPressed(MouseCode button) const override { return m_Data.m_MouseButtons.at(button); };

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		HWND m_Window;
		HDC m_DeviceContextHandler;
		Ref<RenderContext> m_RenderContext;
		WNDCLASSEX m_Class;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool UseImGUI;
			std::unordered_map<KeyCode, bool> m_Keys;
			std::unordered_map<MouseCode, bool> m_MouseButtons;
			HWND* hwnd;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}