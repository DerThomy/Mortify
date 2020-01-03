#include "mtpch.h"

#include "Platform/Windows/WindowsWindow.h"

#include "Mortify/Core/Events/ApplicationEvent.h"
#include "Mortify/Core/Events/MouseEvent.h"
#include "Mortify/Core/Events/KeyEvent.h"
#include "Mortify/Core/MouseCodes.h"
#include "Mortify/Core/KeyCodes.h"

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
		WindowData& data = *(WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (&data != nullptr && data.UseImGUI)
		{
			if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
				return true;
			else if (msg == WM_DPICHANGED)
			{
				if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
				{
					//const int dpi = HIWORD(wParam);
					//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
					const RECT* suggested_rect = (RECT*)lparam;
					::SetWindowPos(*data.hwnd, NULL, suggested_rect->left, suggested_rect->top,
						suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top,
						SWP_NOZORDER | SWP_NOACTIVATE);
				}
			}
		}

		switch (msg)
		{
		case WM_SIZE:
		{
			int width = LOWORD(lparam);
			int height = HIWORD(lparam);

			data.Width = width;
			data.Height = height;

			if (data.EventCallback)
			{
				WindowResizeEvent resizeEvent(width, height);
				data.EventCallback(resizeEvent);
			}
			break;
		}

		case WM_DESTROY:
		{
			if (data.EventCallback)
			{
				WindowCloseEvent closeEvent;
				data.EventCallback(closeEvent);
			}

			PostQuitMessage(0);
			break;
		}
		case WM_KEYDOWN:
		{
			KeyCode key = translateWin32Keys(wparam, lparam);
			data.m_Keys[key] = true;
			if (data.EventCallback)
			{
				KeyPressedEvent keyPressedEvent(key, LOWORD(lparam));
				data.EventCallback(keyPressedEvent);
			}
			break;
		}
		case WM_KEYUP:
		{
			KeyCode key = translateWin32Keys(wparam, lparam);
			data.m_Keys[key] = false;
			if (data.EventCallback)
			{
				KeyReleasedEvent keyReleasedEvent(key);
				data.EventCallback(keyReleasedEvent);
			}
			break;
		}

		case WM_CHAR:
		{
			if (data.EventCallback)
			{
				KeyTypedEvent KeyTypedEvent(win32_keycodes[(unsigned int)wparam]);
				data.EventCallback(KeyTypedEvent);
			}
			break;
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
			if (data.EventCallback)
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
					data.m_MouseButtons[button] = true;
					MouseButtonClickedEvent mouseClickedEvent(button);
					data.EventCallback(mouseClickedEvent);
				}
				else
				{
					data.m_MouseButtons[button] = false;
					MouseButtonReleasedEvent mouseReleasedEvent(button);
					data.EventCallback(mouseReleasedEvent);
				}
			}

			break;
		}

		case WM_MOUSEWHEEL:
		{
			if (data.EventCallback)
			{
				MouseScrolledEvent mouseVScrolledEvent(0.0f, (SHORT)HIWORD(wparam) / (double)WHEEL_DELTA);
				data.EventCallback(mouseVScrolledEvent);
			}
			break;
		}

		case WM_MOUSEHWHEEL:
		{
			if (data.EventCallback)
			{
				MouseScrolledEvent mouseHScrolledEvent((SHORT)HIWORD(wparam) / (double)WHEEL_DELTA, 0.0f);
				data.EventCallback(mouseHScrolledEvent);
			}
			break;
		}

		case WM_MOUSEMOVE:
		{
			const int x = (int)(short)LOWORD(lparam);
			const int y = (int)(short)HIWORD(lparam);

			if (data.EventCallback)
			{
				MouseMovedEvent mouseMovedEvent(x, y);
				data.EventCallback(mouseMovedEvent);
			}

			break;
		}

		case WM_ERASEBKGND:
		{
			return true;
		}

		case WM_SIZING:
		{
			return true;
		}

		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}

		return NULL;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		MT_PROFILE_FUNCTION();

		for (const auto& code : KeyCode())
			m_Data.m_Keys[code] = false;

		for (const auto& button : MouseCode())
			m_Data.m_MouseButtons[button] = false;
		
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		MT_CORE_INFO("Creating window {0} ({1}(w), {2}(h))", props.Title, props.Width, props.Height);

		std::wstring class_name(L"WindowsWindowClass" + std::to_wstring(windowCount));

		m_Class.cbClsExtra = NULL;
		m_Class.cbSize = sizeof(m_Class);
		m_Class.style = CS_HREDRAW | CS_VREDRAW;
		m_Class.cbWndExtra = sizeof(WindowData*);
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

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		m_Window = CreateWindowEx(NULL, class_name.c_str(), converter.from_bytes(props.Title).c_str(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, props.Width, props.Height, NULL, NULL, NULL, NULL);

		MT_CORE_ASSERT(m_Window, "WindowsWindow creation failed");
		m_Data.hwnd = &m_Window;

		windowCount++;
		SetWindowLongPtr(m_Window, GWLP_USERDATA, (LONG_PTR)&(m_Data));

		m_DeviceContextHandler = GetDC(m_Window);
		
		m_RenderContext = WindowsRenderContext::Create(this);
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

	void WindowsWindow::SetVSync(bool enabled)
	{
		MT_PROFILE_FUNCTION();
		
		if (m_RenderContext->SetVsync(enabled))
			m_Data.VSync = enabled;
		else
			MT_CORE_WARN("Failed to set vsync!");
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	inline bool WindowsWindow::IsKeyPressed(KeyCode code) const
	{
		if (code != KeyCode::Invalid)
			return m_Data.m_Keys.at(code);
		else
			MT_CORE_WARN("Invalid key");

		return false;
	}
}