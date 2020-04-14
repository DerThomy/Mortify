#include "mtpch.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Mortify/Core/Events/ApplicationEvent.h"
#include "Mortify/Core/Events/MouseEvent.h"
#include "Mortify/Core/Events/KeyEvent.h"
#include "Mortify/Core/MouseCodes.h"
#include "Mortify/Core/KeyCodes.h"
#include "Mortify/Core/OS.h"

#include <glad/glad.h>
#include <imgui.h>
#include <locale>
#include <codecvt>
#include <memory>
#include <algorithm>

#include <imgui.h>
#include <examples/imgui_impl_win32.h>

namespace Mortify
{
	static uint8_t windowCount = 0;

	WindowsWindow* WindowsWindow::s_MainWindow = nullptr;

	Scope<Window> Window::Create(const WindowProps& props, const EventCallbackFn& callback)
	{
		return CreateScope<WindowsWindow>(props, callback);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props, const EventCallbackFn& callback)
	{
		MT_PROFILE_FUNCTION();

		m_OS = std::dynamic_pointer_cast<WindowsOS>(OS::GetOS());

		if (!s_MainWindow)
			s_MainWindow = this;
		
		if (callback)
			m_EventCallback = callback;

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		MT_PROFILE_FUNCTION();
		
		Shutdown();
	}

	//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowsWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		WindowsWindow* window = (WindowsWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (window != nullptr && window->m_Props.UseImGUI)
		{
			if (ImGui_ImplWin32_WndProcHandler(window->m_Window, msg, wparam, lparam))
				return true;
		}

		if (!window)
		{
			switch(msg)
			{
				case WM_NCCREATE:
				{
					Ref<WindowsOS> os = std::dynamic_pointer_cast<WindowsOS>(OS::GetOS());
						
					if (os->IsWindows10AnniversaryUpdateOrGreater())
						os->GetLibraries().User32.EnableNonClientDpiScaling(hwnd);

					break;
				}
			}

			return DefWindowProc(hwnd, msg, wparam, lparam);
		}

		switch (msg)
		{

		case WM_SIZE:
		{
			int width = LOWORD(lparam);
			int height = HIWORD(lparam);

			window->m_Props.Width = width;
			window->m_Props.Height = height;

			if (wparam == SIZE_MINIMIZED)
			{
				window->m_Props.Maximized = false;
				window->m_Props.Minimized = true;
				window->m_Props.Mode = WindowMode::Minimized;
			}
			else if (wparam == SIZE_MAXIMIZED)
			{
				window->m_Props.Maximized = true;
				window->m_Props.Minimized = false;
				window->m_Props.Mode = WindowMode::Maximized;
			}
			else if (wparam == SIZE_RESTORED)
			{
				window->m_Props.Maximized = false;
				window->m_Props.Minimized = false;
				window->m_Props.Mode = WindowMode::Windowed;
			}

			if (window->m_EventCallback)
			{
				WindowResizeEvent resizeEvent(width, height);
				window->m_EventCallback(resizeEvent);
			}
			return 0;
		}

		case WM_SIZING:
		{
			if (!window->KeepsAspectRatio())
				break;
				
			window->ApplyAspectRatio((int) wparam, (RECT*) lparam);
			return true;
		}

		case WM_GETMINMAXINFO:
        {
            int xoff, yoff;
            UINT dpi = USER_DEFAULT_SCREEN_DPI;
            MINMAXINFO* mmi = (MINMAXINFO*) lparam;

            if (window->m_OS->IsWindows10AnniversaryUpdateOrGreater())
                dpi = GetDpiForWindow(window->m_Window);

            window->GetFullWindowSize(window->GetWindowStyle(), window->GetWindowStyleEx(),
                              0, 0, &xoff, &yoff, dpi);

            if (window->m_Limits.MinWidth.has_value() &&
                window->m_Limits.MinHeight.has_value())
            {
                mmi->ptMinTrackSize.x = window->m_Limits.MinWidth.value() + xoff;
                mmi->ptMinTrackSize.y = window->m_Limits.MinHeight.value() + yoff;
            }

            if (window->m_Limits.MaxWidth.has_value() &&
                window->m_Limits.MaxHeight.has_value())
            {
                mmi->ptMaxTrackSize.x = window->m_Limits.MaxWidth.value() + xoff;
                mmi->ptMaxTrackSize.y = window->m_Limits.MaxHeight.value() + yoff;
            }

            if (window->m_Props.Fullscreen || window->m_Props.Borderless)
            {
                MONITORINFO mi;
                const HMONITOR mh = MonitorFromWindow(window->m_Window,
                                                      MONITOR_DEFAULTTONEAREST);

                ZeroMemory(&mi, sizeof(mi));
                mi.cbSize = sizeof(mi);
                GetMonitorInfo(mh, &mi);

                mmi->ptMaxPosition.x = mi.rcWork.left - mi.rcMonitor.left;
                mmi->ptMaxPosition.y = mi.rcWork.top - mi.rcMonitor.top;
                mmi->ptMaxSize.x = mi.rcWork.right - mi.rcWork.left;
                mmi->ptMaxSize.y = mi.rcWork.bottom - mi.rcWork.top;
            }

            return 0;
        }
			
		case WM_DPICHANGED:
        {
            const float xscale = HIWORD(wparam) / (float) USER_DEFAULT_SCREEN_DPI;
            const float yscale = LOWORD(wparam) / (float) USER_DEFAULT_SCREEN_DPI;

            // Only apply the suggested size if the OS is new enough to have
            // sent a WM_GETDPISCALEDSIZE before this
            if (window->m_OS->IsWindows10CreatorsUpdateOrGreater())
            {
                RECT* suggested = (RECT*) lparam;
                SetWindowPos(window->m_Window, HWND_TOP,
                             suggested->left,
                             suggested->top,
                             suggested->right - suggested->left,
                             suggested->bottom - suggested->top,
                             SWP_NOACTIVATE | SWP_NOZORDER);
            }

            break;
        }

		case WM_GETDPISCALEDSIZE:
        {
            // Adjust the window size to keep the content area size constant
            if (window->m_OS->IsWindows10CreatorsUpdateOrGreater())
            {
                RECT source = {0}, target = {0};
                SIZE* size = (SIZE*) lparam;

                AdjustWindowRectExForDpi(&source, window->GetWindowStyle(),
                                         FALSE, window->GetWindowStyleEx(),
                                         GetDpiForWindow(window->m_Window));
                AdjustWindowRectExForDpi(&target, window->GetWindowStyle(),
                                         FALSE, window->GetWindowStyleEx(),
                                         LOWORD(wparam));

                size->cx += (target.right - target.left) -
                            (source.right - source.left);
                size->cy += (target.bottom - target.top) -
                            (source.bottom - source.top);
                return TRUE;
            }

            break;
        }

		case WM_SETFOCUS:
		{
			break;
		}
			
		case WM_KILLFOCUS:
		{
			break;
		}
			
		case WM_NCACTIVATE:
		case WM_NCPAINT:
		{
			if (window->m_Props.Fullscreen || window->m_Props.Borderless)
				return true;

			break;
		}

		case WM_DESTROY:
		{
			if (window->m_EventCallback)
			{
				WindowCloseEvent closeEvent;
				window->m_EventCallback(closeEvent);
			}

			PostQuitMessage(0);
			return 0;
		}
		case WM_SYSCOMMAND:
		{
			switch (wparam & 0xfff0)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				{
					if (window->m_Props.Mode == WindowMode::Fullscreen)
					{
						// We are running in full screen mode, so disallow
						// screen saver and screen blanking
						return 0;
					}
					
					break;
				}

				// User trying to access application menu using ALT?
				case SC_KEYMENU:
					return 0;
			}

			break;
		}
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			KeyCode key = translateWin32Keys(wparam, lparam);
				
			if (key == KeyCode::Invalid)
				break;

			if (wparam == VK_SNAPSHOT)
			{
				window->m_Keys[key] = false;
				
				if (window->m_EventCallback)
				{
					KeyPressedEvent snapPressed(key, LOWORD(lparam));
					window->m_EventCallback(snapPressed);
					KeyReleasedEvent snapReleased(key);
					window->m_EventCallback(snapReleased);
				}

				window->m_Keys[key] = true;
				
				break;
			}
				
			window->m_Keys[key] = true;
			if (window->m_EventCallback)
			{
				KeyPressedEvent keyPressedEvent(key, LOWORD(lparam));
				window->m_EventCallback(keyPressedEvent);
			}
				
			break;
		}
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			KeyCode key = translateWin32Keys(wparam, lparam);
				
			if (key == KeyCode::Invalid)
				break;

			// Release both shift keys because if both are pressed the first release does not emit an event
			if (wparam == VK_SHIFT)
			{
				window->m_Keys[MT_KEY_LEFT_SHIFT] = false;
				window->m_Keys[MT_KEY_RIGHT_SHIFT] = false;
				
				if (window->m_EventCallback)
				{
					KeyReleasedEvent keyLeftShift(MT_KEY_LEFT_SHIFT);
					window->m_EventCallback(keyLeftShift);
					KeyReleasedEvent keyRightShift(MT_KEY_RIGHT_SHIFT);
					window->m_EventCallback(keyRightShift);
				}
				
				break;
			}
				
			window->m_Keys[key] = false;
			if (window->m_EventCallback)
			{
				KeyReleasedEvent keyReleasedEvent(key);
				window->m_EventCallback(keyReleasedEvent);
			}
				
			break;
		}

		case WM_CHAR:
		{
			if (window->m_EventCallback)
			{
				KeyTypedEvent KeyTypedEvent(win32_keycodes[(unsigned int)wparam]);
				window->m_EventCallback(KeyTypedEvent);
			}
			return 0;
		}

		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_XBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONUP:
		{
			if (window->m_EventCallback)
			{
				MouseCode button;

				if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONUP)
					button = MT_MOUSE_BUTTON_LEFT;
				else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONUP)
					button = MT_MOUSE_BUTTON_RIGHT;
				else if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONUP)
					button = MT_MOUSE_BUTTON_MIDDLE;
				else if (GET_XBUTTON_WPARAM(wparam) == XBUTTON1)
					button = MT_MOUSE_BUTTON_4;
				else
					button = MT_MOUSE_BUTTON_5;

				if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN ||
					msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN)
				{
					window->m_MouseButtons[button] = true;
					MouseButtonClickedEvent mouseClickedEvent(button);
					window->m_EventCallback(mouseClickedEvent);
				}
				else
				{
					window->m_MouseButtons[button] = false;
					MouseButtonReleasedEvent mouseReleasedEvent(button);
					window->m_EventCallback(mouseReleasedEvent);
				}
			}

			if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONUP)
				return true;

return 0;
		}

		case WM_MOUSEWHEEL:
		{
			if (window->m_EventCallback)
			{
				MouseScrolledEvent mouseVScrolledEvent(0.0f, (SHORT)HIWORD(wparam) / (float)WHEEL_DELTA);
				window->m_EventCallback(mouseVScrolledEvent);
			}
			return 0;
		}

		case WM_MOUSEHWHEEL:
		{
			if (window->m_EventCallback)
			{
				MouseScrolledEvent mouseHScrolledEvent((SHORT)HIWORD(wparam) / (float)WHEEL_DELTA, 0.0f);
				window->m_EventCallback(mouseHScrolledEvent);
			}
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			const float x = (float)(short)LOWORD(lparam);
			const float y = (float)(short)HIWORD(lparam);

			if (window->m_EventCallback)
			{
				MouseMovedEvent mouseMovedEvent(x, y);
				window->m_EventCallback(mouseMovedEvent);
			}

			return 0;
		}

		case WM_ERASEBKGND:
		{
			return true;
		}
		}

		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	void WindowsWindow::SetBorderless(bool borderless)
	{
		m_Props.Borderless = true;

		UpdateWindowStyle();
	}

	void WindowsWindow::Close()
	{
		SendMessage(m_Window, WM_SYSCOMMAND, SC_CLOSE, 0);
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		MT_PROFILE_FUNCTION();

		for (const auto& code : KeyCode())
			m_Keys[code] = false;

		for (const auto& button : MouseCode())
			m_MouseButtons[button] = false;

		m_Props = props;

		if ((m_Limits.MaxHeight.has_value() && m_Limits.MaxHeight.value() > m_Props.Height)
			|| (m_Limits.MaxWidth.has_value() && m_Limits.MaxWidth.value() > m_Props.Width)
			|| (m_Limits.MinHeight.has_value() && m_Limits.MinHeight.value() < m_Props.Height)
			|| (m_Limits.MinWidth.has_value() && m_Limits.MinWidth.value() < m_Props.Width))
		{
			MT_CORE_WARN("Window {0}: Sizes are outside specified Limits and will thus be clipped to those Limits!", windowCount);
			ClipSize();
		}

		std::wstring class_name(L"WindowsWindowClass" + std::to_wstring(windowCount));
		int xpos, ypos, fullwidth, fullheight;
		DWORD style = GetWindowStyle();
		DWORD exstyle = GetWindowStyleEx();

		MT_CORE_INFO("Creating window {0} ({1}(w), {2}(h))", m_Props.Title, m_Props.Width, m_Props.Height);

		HMONITOR primaryMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);

		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(primaryMonitor, &mi);

		// If Fullscreen set Saved Info to window mode
		if (m_Props.Fullscreen)
		{
			GetFullWindowSize(GetWindowStyle(false, m_Props.Borderless, m_Props.Resizeable, m_Props.Maximized),
				GetWindowStyleEx(false), m_Props.Width, m_Props.Height, &fullwidth, &fullheight, m_OS->GetDpiForMonitor(primaryMonitor));

			xpos = (mi.rcMonitor.right - fullwidth) / 2;
			ypos = (mi.rcMonitor.bottom - fullheight) / 2;

			m_SavedInfo.Rect = { xpos, ypos, xpos + static_cast<LONG>(fullwidth), ypos + static_cast<LONG>(fullheight) };
			m_SavedInfo.Maximized = m_Props.Maximized;
			
			m_Props.Width = fullwidth = mi.rcMonitor.right;
			m_Props.Height = fullheight = mi.rcMonitor.bottom;
			xpos = 0;
			ypos = 0;
		}
		else
		{
			GetFullWindowSize(style, exstyle, m_Props.Width, m_Props.Height, &fullwidth, &fullheight, m_OS->GetDpiForMonitor(primaryMonitor));

			xpos = (mi.rcMonitor.right - fullwidth) / 2;
			ypos = (mi.rcMonitor.bottom - fullheight) / 2;
		}

		// Create Window Class
		m_Class.cbClsExtra = NULL;
		m_Class.cbSize = sizeof(m_Class);
		m_Class.style = CS_HREDRAW | CS_VREDRAW;
		m_Class.cbWndExtra = sizeof(WindowsWindow*);
		m_Class.cbClsExtra = NULL;
		m_Class.hbrBackground = (HBRUSH)COLOR_WINDOW;
		m_Class.hCursor = LoadCursor(NULL, IDC_ARROW);
		m_Class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_Class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		m_Class.hInstance = NULL;
		m_Class.lpszClassName = class_name.c_str();
		m_Class.lpszMenuName = L"MainMenu";
		m_Class.lpfnWndProc = WindowsWindow::WndProc;

		MT_ASSERT(RegisterClassEx(&m_Class), "Failed to register class");

		// Create Window
		std::wstring title = m_OS->WideCharFromUTF8(props.Title);
		m_Window = CreateWindowEx(exstyle, class_name.c_str(), title.c_str(),
			style, xpos, ypos, fullwidth, fullheight, NULL, NULL, GetModuleHandleW(NULL), NULL);
		MT_CORE_ASSERT(m_Window, "WindowsWindow creation failed");

		RECT rect;
		GetClientRect(m_Window, &rect);

		// Set Window User Data to this so the Object is available for the WNDProc function
		SetWindowLongPtr(m_Window, GWLP_USERDATA, (LONG_PTR)this);

		// Init RenderContext
		// TODO: Make this controllable
		m_DeviceContextHandler = GetDC(m_Window);
		
		m_RenderContext = RenderContext::Create(this);
		m_RenderContext->Init();

		ShowWindow(m_Window, SW_SHOW);

		// TODO: Make controllable
		SetForegroundWindow(m_Window);
		SetFocus(m_Window);

		if (m_Props.Fullscreen)
			MarkFullscreen(true);

		windowCount++;
	}

	void WindowsWindow::OnUpdate()
	{
		MT_PROFILE_FUNCTION();
		
		MSG msg = { };

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		m_RenderContext->SwapBuffers();
	}

	void WindowsWindow::SetWindowMode(WindowMode mode)
	{
		if (m_Props.Mode == mode)
			return;

		if (mode != WindowMode::Fullscreen && m_Props.Mode == WindowMode::Fullscreen)
		{
			m_Props.Fullscreen = false;

			m_Props.Maximized = m_SavedInfo.Maximized;
			UpdateWindowStyle(m_SavedInfo.Rect);

			MarkFullscreen(false);
		}

		if (mode == WindowMode::Maximized)
		{
			ShowWindow(m_Window, SW_MAXIMIZE);
			m_Props.Maximized = true;
			m_Props.Minimized = false;
		}

		if (mode == WindowMode::Minimized)
		{
			ShowWindow(m_Window, SW_MINIMIZE);
			m_Props.Maximized = false;
			m_Props.Minimized = true;
		}

		if (mode == WindowMode::Windowed)
		{
			ShowWindow(m_Window, SW_RESTORE);
			m_Props.Maximized = false;
			m_Props.Minimized = false;
		}

		if (mode == WindowMode::Fullscreen)
		{
			if (m_Props.Minimized)
				ShowWindow(m_Window, SW_RESTORE);
			
			m_SavedInfo.Maximized = m_Props.Maximized;

			GetWindowRect(m_Window, &m_SavedInfo.Rect);

			m_Props.Fullscreen = true;
			m_Props.Maximized = false;
			m_Props.Minimized = false;

			MONITORINFO mi = { sizeof(mi) };
			GetMonitorInfo(MonitorFromWindow(m_Window, MONITOR_DEFAULTTONEAREST), &mi);
			UpdateWindowStyle(mi.rcMonitor);

			MarkFullscreen(false);
		}

		m_Props.Mode = mode;
	}


	void WindowsWindow::FitToMonitor()
	{
	    MONITORINFO mi = { sizeof(mi) };
	    GetMonitorInfo(MonitorFromWindow(m_Window, MONITOR_DEFAULTTONEAREST), &mi);
	    SetWindowPos(m_Window, HWND_TOPMOST,
	                 mi.rcMonitor.left,
	                 mi.rcMonitor.top,
	                 mi.rcMonitor.right - mi.rcMonitor.left,
	                 mi.rcMonitor.bottom - mi.rcMonitor.top,
	                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS);
	}

	void WindowsWindow::GetFullWindowSize(DWORD style, DWORD exStyle, int contentWidth, int contentHeight, int* fullWidth, int* fullHeight, UINT dpi) const
	{
		RECT rect = { 0, 0, (LONG)contentWidth, (LONG)contentHeight };

		if (m_OS->IsWindows10AnniversaryUpdateOrGreater())
		{
			AdjustWindowRectExForDpi(&rect, style, FALSE, exStyle, dpi);
		}
		else
			AdjustWindowRectEx(&rect, style, FALSE, exStyle);

		*fullWidth = rect.right - rect.left;
		*fullHeight = rect.bottom - rect.top;
	}

	void WindowsWindow::ClipSize()
	{
		if (m_Limits.MinWidth.has_value())
			m_Props.Width = std::max(m_Props.Width, m_Limits.MinWidth.value());
		if (m_Limits.MaxWidth.has_value())
			m_Props.Width = std::min(m_Props.Width, m_Limits.MaxWidth.value());

		if (m_Limits.MinHeight.has_value())
			m_Props.Height = std::max(m_Props.Height, m_Limits.MinHeight.value());
		if (m_Limits.MaxHeight.has_value())
			m_Props.Height = std::min(m_Props.Height, m_Limits.MaxHeight.value());
	}

	void WindowsWindow::MarkFullscreen(bool fullscreen)
	{
		if (m_OS->GetTaskbarList())
			m_OS->GetTaskbarList()->MarkFullscreenWindow(m_Window, !!fullscreen);

		m_Props.Fullscreen = fullscreen;
	}

	void WindowsWindow::MarkFullscreen()
	{
		MarkFullscreen(m_Props.Fullscreen);
	}

	void WindowsWindow::Shutdown()
	{
		MT_PROFILE_FUNCTION();
		
		if (m_Window && !IsWindow(m_Window))
			DestroyWindow(m_Window);

		--windowCount;

		if (s_MainWindow == this)
			s_MainWindow = nullptr;

		m_RenderContext->Destroy();
		ReleaseDC(m_Window, m_DeviceContextHandler);
		UnregisterClassW(m_Class.lpszClassName, m_Class.hInstance);

		//if (windowCount == 0)
			//FreeLibrary(openGLModule);
	}

	DWORD WindowsWindow::GetWindowStyle(bool fullscreen, bool borderless, bool resizeable, bool maximized)
	{
		DWORD style = 0;

		if (fullscreen || borderless) {
			style |= WS_POPUP;
		}
		else {
			if (resizeable) {
				if (maximized) {
					style = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
				}
				else {
					style = WS_OVERLAPPEDWINDOW;
				}
			}
			else {
				style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
			}
		}

		style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		return style;
	}

	DWORD WindowsWindow::GetWindowStyle()
	{
		return GetWindowStyle(m_Props.Fullscreen, m_Props.Borderless, m_Props.Resizeable, m_Props.Maximized);
	}

	DWORD WindowsWindow::GetWindowStyleEx(bool fullscreen)
	{
		DWORD style_ex = WS_EX_WINDOWEDGE;

		if (this == s_MainWindow)
			style_ex |= WS_EX_APPWINDOW;

		if (fullscreen)
			style_ex |= WS_EX_TOPMOST;

		return style_ex;
	}

	DWORD WindowsWindow::GetWindowStyleEx()
	{
		return GetWindowStyleEx(m_Props.Fullscreen);
	}

	void WindowsWindow::ApplyAspectRatio(int edge, RECT* area)
	{
		int xoff, yoff;
		UINT dpi = USER_DEFAULT_SCREEN_DPI;
		float ratio = (float) m_Props.Width / (float) m_Props.Height;

		if(m_OS->IsWindows10AnniversaryUpdateOrGreater())
			GetDpiForWindow(m_Window);

		GetFullWindowSize(GetWindowStyle(), GetWindowStyleEx(), 0,
			0, &xoff, &yoff, dpi);

		if (edge == WMSZ_LEFT  || edge == WMSZ_BOTTOMLEFT ||
        edge == WMSZ_RIGHT || edge == WMSZ_BOTTOMRIGHT)
	    {
	        area->bottom = area->top + yoff +
	            (int) ((area->right - area->left - xoff) / ratio);
	    }
	    else if (edge == WMSZ_TOPLEFT || edge == WMSZ_TOPRIGHT)
	    {
	        area->top = area->bottom - yoff -
	            (int) ((area->right - area->left - xoff) / ratio);
	    }
	    else if (edge == WMSZ_TOP || edge == WMSZ_BOTTOM)
	    {
	        area->right = area->left + xoff +
	            (int) ((area->bottom - area->top - yoff) * ratio);
	    }
	}

	void WindowsWindow::UpdateWindowStyle(bool repaint)
	{		
		DWORD style = GetWindowStyle();
		DWORD style_ex = GetWindowStyleEx();

		SetWindowLong(m_Window, GWL_STYLE, style);
		SetWindowLong(m_Window, GWL_EXSTYLE, style_ex);

		SetWindowPos(m_Window, HWND_TOP, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

		if (repaint)
		{
			RECT rect;
			GetWindowRect(m_Window, &rect);
			MoveWindow(m_Window, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
	}

	void WindowsWindow::UpdateWindowStyle(RECT rect)
	{
		UpdateWindowStyle(false);

		MoveWindow(m_Window, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		MT_PROFILE_FUNCTION();
		
		if (m_RenderContext->SetVsync(enabled))
			m_Props.VSync = enabled;
		else
			MT_CORE_WARN("Failed to set vsync!");
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Props.VSync;
	}

	inline bool WindowsWindow::IsKeyPressed(KeyCode code) const
	{
		if (code != KeyCode::Invalid)
			return m_Keys.at(code);
		else
			MT_CORE_WARN("Invalid key");

		return false;
	}
}