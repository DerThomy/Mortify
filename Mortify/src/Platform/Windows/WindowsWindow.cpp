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

#include <imgui.h>
#include <examples/imgui_impl_win32.h>

namespace Mortify
{
	static uint8_t windowCount = 0;

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		MT_PROFILE_FUNCTION();

		m_OS = std::dynamic_pointer_cast<WindowsOS>(OS::GetOS());
		
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

		if (window != nullptr && window->m_UsesImGUI)
		{
			if (ImGui_ImplWin32_WndProcHandler(window->m_Window, msg, wparam, lparam))
				return true;
			else if (msg == WM_DPICHANGED)
			{
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
				{
					//const int dpi = HIWORD(wParam);
					//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
					const RECT* suggested_rect = (RECT*)lparam;
					::SetWindowPos(window->m_Window, NULL, suggested_rect->left, suggested_rect->top,
						suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top,
						SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
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
			const bool maximized = wparam == SIZE_MAXIMIZED ||
                                       (window->m_Maximized &&
                                        wparam != SIZE_RESTORED);

			window->m_Maximized = maximized;
				
			int width = LOWORD(lparam);
			int height = HIWORD(lparam);

			window->m_Width = width;
			window->m_Height = height;

			if (window->m_EventCallback)
			{
				WindowResizeEvent resizeEvent(width, height);
				window->m_EventCallback(resizeEvent);
			}
			return 0;
		}

		case WM_SIZING:
		{
			if (!window->KeepAspectRatio())
				break;
				
			window->ApplyAspectRatio((int) wparam, (RECT*) lparam);
			return true;
		}

		case WM_GETMINMAXINFO:
        {
            int xoff, yoff;
            UINT dpi = USER_DEFAULT_SCREEN_DPI;
            MINMAXINFO* mmi = (MINMAXINFO*) lparam;

            if (window->GetWindowMode() == WindowMode::Fullscreen)
                break;

            if (window->m_OS->IsWindows10AnniversaryUpdateOrGreater())
                dpi = GetDpiForWindow(window->m_Window);

            window->GetFullWindowSize(window->GetWindowStyle(), window->GetWindowStyleEx(),
                              0, 0, &xoff, &yoff, dpi);

            if (window->m_Limits.MinWidth != MT_DONT_CARE &&
                window->m_Limits.MinHeight != MT_DONT_CARE)
            {
                mmi->ptMinTrackSize.x = window->m_Limits.MinWidth + xoff;
                mmi->ptMinTrackSize.y = window->m_Limits.MinHeight + yoff;
            }

            if (window->m_Limits.MaxWidth != MT_DONT_CARE &&
                window->m_Limits.MaxHeight != MT_DONT_CARE)
            {
                mmi->ptMaxTrackSize.x = window->m_Limits.MaxWidth + xoff;
                mmi->ptMaxTrackSize.y = window->m_Limits.MaxHeight + yoff;
            }

            if (!window->m_Decorated)
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
			if (!window->m_Decorated)
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
					if (window->m_Mode == WindowMode::Fullscreen)
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

	void WindowsWindow::Maximize()
	{
		if (!m_Maximized && m_Mode != WindowMode::Fullscreen)
			ShowWindow(m_Window, SW_MAXIMIZE);
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		MT_PROFILE_FUNCTION();

		for (const auto& code : KeyCode())
			m_Keys[code] = false;

		for (const auto& button : MouseCode())
			m_MouseButtons[button] = false;
		
		m_Title = props.Title;
		m_Width = props.Width;
		m_Height = props.Height;
		m_Maximized = props.Maximized;
		m_Resizable = props.Resizeable;
		m_KeepAspect = props.KeepAspect;
		m_Mode = props.Mode;

		m_Limits = WindowLimits();

		MT_CORE_INFO("Creating window {0} ({1}(w), {2}(h))", props.Title, props.Width, props.Height);

		std::wstring class_name(L"WindowsWindowClass" + std::to_wstring(windowCount));
		int xpos, ypos, width, height;
		DWORD style = GetWindowStyle();
		DWORD exstyle = GetWindowStyleEx();

		xpos = CW_USEDEFAULT;
		ypos = CW_USEDEFAULT;

		if (m_Maximized)
			style |= WS_MAXIMIZE;

		GetFullWindowSize(style, exstyle, props.Width, props.Height, &width, &height, USER_DEFAULT_SCREEN_DPI);

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

		std::wstring title = m_OS->WideCharFromUTF8(props.Title);
		m_Window = CreateWindowEx(exstyle, class_name.c_str(), title.c_str(),
			style, xpos, ypos, width, height, NULL, NULL, GetModuleHandleW(NULL), NULL);
		MT_CORE_ASSERT(m_Window, "WindowsWindow creation failed");

		windowCount++;
		SetWindowLongPtr(m_Window, GWLP_USERDATA, (LONG_PTR)this);

		m_DeviceContextHandler = GetDC(m_Window);
		
		m_RenderContext = RenderContext::Create(this);
		m_RenderContext->Init();

		ShowWindow(m_Window, SW_SHOW);
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
		m_Mode = mode;
		m_Decorated = mode == WindowMode::Fullscreen || mode == WindowMode::Borderless ? false : true;

		if (mode == WindowMode::Fullscreen)
		{
			MONITORINFO mi = { sizeof(mi) };
			UINT flags = SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOCOPYBITS;

	        if (m_Decorated)
	        {
	            DWORD style = GetWindowLongW(m_Window, GWL_STYLE);
	            style &= ~WS_OVERLAPPEDWINDOW;
	            style |= GetWindowStyle();
	            SetWindowLongW(m_Window, GWL_STYLE, style);
	            flags |= SWP_FRAMECHANGED;
	        }

			GetMonitorInfo(MonitorFromWindow(m_Window, MONITOR_DEFAULTTONEAREST), &mi);
	        SetWindowPos(m_Window, HWND_TOPMOST,
	                     mi.rcMonitor.left,
	                     mi.rcMonitor.top,
	                     mi.rcMonitor.right - mi.rcMonitor.left,
	                     mi.rcMonitor.bottom - mi.rcMonitor.top,
	                     flags);
		}
		else
		{
			/*
			HWND after;
	        RECT rect = { xpos, ypos, xpos + width, ypos + height };
	        DWORD style = GetWindowLongW(window->win32.handle, GWL_STYLE);
	        UINT flags = SWP_NOACTIVATE | SWP_NOCOPYBITS;

	        if (window->decorated)
	        {
	            style &= ~WS_POPUP;
	            style |= getWindowStyle(window);
	            SetWindowLongW(window->win32.handle, GWL_STYLE, style);

	            flags |= SWP_FRAMECHANGED;
	        }

	        if (window->floating)
	            after = HWND_TOPMOST;
	        else
	            after = HWND_NOTOPMOST;

	        if (_glfwIsWindows10AnniversaryUpdateOrGreaterWin32())
	        {
	            AdjustWindowRectExForDpi(&rect, getWindowStyle(window),
	                                     FALSE, getWindowExStyle(window),
	                                     GetDpiForWindow(window->win32.handle));
	        }
	        else
	        {
	            AdjustWindowRectEx(&rect, getWindowStyle(window),
	                               FALSE, getWindowExStyle(window));
	        }

	        SetWindowPos(window->win32.handle, after,
	                     rect.left, rect.top,
	                     rect.right - rect.left, rect.bottom - rect.top,
	                     flags);
	                     */
		}
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
		RECT rect = { 0, 0, contentWidth, contentHeight };

		if (m_OS->IsWindows10AnniversaryUpdateOrGreater())
			AdjustWindowRectExForDpi(&rect, style, FALSE, exStyle, dpi);
		else
			AdjustWindowRectEx(&rect, style, FALSE, exStyle);

		*fullWidth = rect.right - rect.left;
		*fullHeight = rect.bottom - rect.top;
	}

	void WindowsWindow::Shutdown()
	{
		MT_PROFILE_FUNCTION();
		
		if (m_Window)
			DestroyWindow(m_Window);

		--windowCount;

		m_RenderContext->Destroy();
		ReleaseDC(m_Window, m_DeviceContextHandler);
		UnregisterClassW(m_Class.lpszClassName, m_Class.hInstance);

		//if (windowCount == 0)
			//FreeLibrary(openGLModule);
	}

	DWORD WindowsWindow::GetWindowStyle()
	{
		DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		if (m_Mode == WindowMode::Fullscreen)
			style |= WS_POPUP;
		else
		{
			style |= WS_SYSMENU | WS_MINIMIZEBOX;

			if (m_Decorated)
			{
				style |= WS_CAPTION;

				if (m_Resizable)
					style |= WS_MAXIMIZEBOX | WS_THICKFRAME;
			}
			else
				style |= WS_POPUP;
		}

		return style;
	}

	DWORD WindowsWindow::GetWindowStyleEx()
	{
		DWORD style = WS_EX_APPWINDOW;

		if (m_Mode == WindowMode::Fullscreen)
			style |= WS_EX_TOPMOST;

		return style;
	}

	void WindowsWindow::ApplyAspectRatio(int edge, RECT* area)
	{
		int xoff, yoff;
		UINT dpi = USER_DEFAULT_SCREEN_DPI;
		float ratio = (float) m_Width / (float) m_Height;

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

	void WindowsWindow::UpdateWindowStyle()
	{
		RECT rect;
		DWORD style = GetWindowLongW(m_Window, GWL_STYLE);
		style |= ~(WS_OVERLAPPEDWINDOW | WS_POPUP);
		style |= GetWindowStyle();

		GetClientRect(m_Window, &rect);

		if (m_OS->IsWindows10AnniversaryUpdateOrGreater())
		{
			AdjustWindowRectExForDpi(&rect, style, FALSE,
				GetWindowStyleEx(),
				GetDpiForWindow(m_Window));
		}
		else
			AdjustWindowRectEx(&rect, style, FALSE, GetWindowStyleEx());

		ClientToScreen(m_Window, (POINT*) &rect.left);
		ClientToScreen(m_Window, (POINT*) &rect.right);
		SetWindowLongW(m_Window, GWL_STYLE, style);
		SetWindowPos(m_Window, HWND_TOP, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		MT_PROFILE_FUNCTION();
		
		if (m_RenderContext->SetVsync(enabled))
			m_VSync = enabled;
		else
			MT_CORE_WARN("Failed to set vsync!");
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_VSync;
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