#include "mtpch.h"

#include "Platform/Linux/LinuxWindow.h"

#include "Mortify/Core/WindowManager.h"
#include "Mortify/Core/Events/ApplicationEvent.h"
#include "Mortify/Core/Events/MouseEvent.h"
#include "Mortify/Core/Events/KeyEvent.h"
#include "Mortify/Core/Events/WindowEvent.h"
#include "Mortify/Core/MouseCodes.h"
#include "Mortify/Core/KeyCodes.h"
#include "Mortify/Core/OS.h"

#include "Mortify/Rendering/Renderer.h"

namespace Mortify
{
	LinuxWindow* LinuxWindow::s_MainWindow = nullptr;
    
    static void GLFWErrorCallback(int error, const char* description)
	{
		MT_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Ref<Window> WindowManager::CreatePlatformWindow(WindowID id, const WindowConfig& config, const EventCallbackFn& callback)
	{
		return CreateRef<LinuxWindow>(id, config, callback);
	}

	LinuxWindow::LinuxWindow(WindowID id, const WindowConfig& config, const EventCallbackFn& callback)
		: m_Props(id, config, callback)
	{
		MT_PROFILE_FUNCTION();

		MT_CORE_ASSERT(config.Width > 0, "Width out of range!");
		MT_CORE_ASSERT(config.Height > 0, "Height out of range!");

		m_OS = std::dynamic_pointer_cast<LinuxOS>(OS::GetOS());

		if (!s_MainWindow)
			s_MainWindow = this;

		Init();
	}

	LinuxWindow::~LinuxWindow()
	{
		MT_PROFILE_FUNCTION();
		
		Shutdown();
	}

	void LinuxWindow::Close()
	{
        MT_PROFILE_FUNCTION();
        
		glfwDestroyWindow(m_WindowHandle);
        
        if (WindowManager::GetWindowCount() == 1)
            glfwTerminate();
	}
    
    void LinuxWindow::Shutdown()
    {
        Close();
    }

	void LinuxWindow::Init()
	{
        MT_PROFILE_FUNCTION();
        
        for (const auto& code : KeyCode())
			m_Keys[code] = false;

		for (const auto& button : MouseCode())
			m_MouseButtons[button] = false;

		if ((m_Limits.MaxHeight.has_value() && m_Limits.MaxHeight.value() < m_Props.Height)
			|| (m_Limits.MaxWidth.has_value() && m_Limits.MaxWidth.value() < m_Props.Width)
			|| (m_Limits.MinHeight.has_value() && m_Limits.MinHeight.value() > m_Props.Height)
			|| (m_Limits.MinWidth.has_value() && m_Limits.MinWidth.value() > m_Props.Width))
		{
			MT_CORE_WARN("Window {0}: Sizes are outside specified Limits and will thus be clipped to those Limits!", m_Props.ID);
			ClipSize();
		}
        
        uint16_t windowCount = WindowManager::GetWindowCount();
        
        if (windowCount == 0)
		{
			MT_PROFILE_SCOPE("glfwInit");
            
			int success = glfwInit();
			MT_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}
        
        {
			MT_PROFILE_SCOPE("glfwCreateWindow");
            
            #if defined(MT_DEBUG)
                if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
                    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
            #endif
            
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
			m_WindowHandle = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), nullptr, nullptr);
		}
        
        glfwSetWindowSizeLimits(m_WindowHandle, m_Limits.MinWidth.has_value() ? m_Limits.MinWidth.value() : GLFW_DONT_CARE,
                m_Limits.MinHeight.has_value() ? m_Limits.MinHeight.value() : GLFW_DONT_CARE,
                m_Limits.MaxWidth.has_value() ? m_Limits.MaxWidth.value() : GLFW_DONT_CARE,
                m_Limits.MaxHeight.has_value() ? m_Limits.MaxHeight.value() : GLFW_DONT_CARE
        );
                
        m_RenderContext = RenderContext::Create(this);
        m_RenderContext->Init();
        
        SetVSync(m_Props.VSync);
        
        glfwSetWindowUserPointer(m_WindowHandle, &m_Props);
        
        glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* glfwWindow, int width, int height)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(glfwWindow);
			props.Width = width;
			props.Height = height;

			WindowResizeEvent event(props.ID, width, height);
			props.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* glfwWindow)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(glfwWindow);
			WindowCloseEvent event(props.ID);
			props.EventCallback(event);
		});

		glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(glfwWindow);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event((KeyCode)key, 0);
					props.EventCallback(event);
                    //m_Keys.at(key) = true;
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event((KeyCode)key);
					props.EventCallback(event);
                    //m_Keys.at(key) = false;
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event((KeyCode)key, 1);
					props.EventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_WindowHandle, [](GLFWwindow* glfwWindow, unsigned int keycode)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(glfwWindow);

			KeyTypedEvent event((KeyCode)keycode);
			props.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(glfwWindow);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonClickedEvent event(props.ID, (MouseCode)button);
					props.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(props.ID, (MouseCode)button);
					props.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* glfwWindow, double xOffset, double yOffset)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(glfwWindow);

			MouseScrolledEvent event(props.ID, (float)xOffset, (float)yOffset);
			props.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* glfwWindow, double xPos, double yPos)
		{
			WindowProps& props = *(WindowProps*)glfwGetWindowUserPointer(glfwWindow);

			MouseMovedEvent event(props.ID, (float)xPos, (float)yPos);
			props.EventCallback(event);
		});
	}
    
    void LinuxWindow::ClipSize()
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
    
    void LinuxWindow::OnUpdate()
	{
		MT_PROFILE_FUNCTION();
        
		glfwPollEvents();
		m_RenderContext->SwapBuffers();
	}

	inline WindowMode LinuxWindow::GetWindowMode() const
	{
		if (m_Props.Maximized)
			return WindowMode::Maximized;
		else if (m_Props.Minimized)
			return WindowMode::Minimized;
		else if (m_Props.Fullscreen)
			return WindowMode::Fullscreen;
		else
			return WindowMode::Windowed;
	}
    
    void LinuxWindow::SetFlags(uint8_t flags, bool state)
	{
		
		if (flags & WindowFlag::Borderless)
		{
			m_Props.Borderless = state;
			glfwWindowHint(GLFW_DECORATED, state ? GLFW_TRUE : GLFW_FALSE);
		}
		if (flags & WindowFlag::KeepAspectRatio)
		{
			m_Props.KeepAspect = state;
            glfwSetWindowAspectRatio(m_WindowHandle, state ? m_Props.Width : GLFW_DONT_CARE, state ? m_Props.Height : GLFW_DONT_CARE);
		}
		if (flags & WindowFlag::AlwaysOnTop)
		{
			m_Props.AlwaysOnTop = state;
			glfwWindowHint(GLFW_FLOATING, state ? GLFW_TRUE : GLFW_FALSE);
		}
		if (flags & WindowFlag::AutoIconify)
		{
			m_Props.AutoIconify = state;
            glfwWindowHint(GLFW_AUTO_ICONIFY, state ? GLFW_TRUE : GLFW_FALSE);
		}
		if (flags & WindowFlag::DisableResize)
		{
			m_Props.Resizeable = !state;
			glfwWindowHint(GLFW_RESIZABLE, state ? GLFW_FALSE : GLFW_TRUE);
		}
		if (flags & WindowFlag::ScaleToMonitor)
		{
			m_Props.ScaleToMonitor = state;
            glfwWindowHint(GLFW_SCALE_TO_MONITOR, state ? GLFW_TRUE : GLFW_FALSE);
		}
	}

	void LinuxWindow::SetWindowMode(WindowMode mode)
	{
		if (GetWindowMode() == mode)
			return;

		if (mode != WindowMode::Fullscreen && m_Props.Fullscreen)
		{
            // exit fullscreen
            glfwSetWindowMonitor( m_WindowHandle, nullptr , m_SavedInfo.XPos, m_SavedInfo.YPos, m_SavedInfo.Width, m_SavedInfo.Height, 0 );
            m_Props.Fullscreen = false;
		}

		if (mode == WindowMode::Maximized)
		{
            glfwMaximizeWindow(m_WindowHandle);
			m_Props.Maximized = true;
			m_Props.Minimized = false;
		}

		if (mode == WindowMode::Minimized)
		{
			glfwIconifyWindow(m_WindowHandle);
			m_Props.Maximized = false;
			m_Props.Minimized = true;
		}

		if (mode == WindowMode::Windowed)
		{
			glfwRestoreWindow(m_WindowHandle);
			m_Props.Maximized = false;
			m_Props.Minimized = false;
		}

		if (mode == WindowMode::Fullscreen)
		{
            glfwGetWindowPos(m_WindowHandle, &m_SavedInfo.XPos, &m_SavedInfo.YPos);
            glfwGetWindowSize(m_WindowHandle, &m_SavedInfo.Width, &m_SavedInfo.Height);
            
            // Find closest monitor
            GLFWmonitor* bestmonitor;
            GLFWmonitor** monitors;
            const GLFWvidmode* monitor_mode;
            int mx, my, mw, mh;
            int wx, wy, ww, wh;
            int nmonitors, i, overlap, bestoverlap;
            
            wx = m_SavedInfo.XPos;
            wy = m_SavedInfo.YPos;
            ww = m_SavedInfo.Width;
            wh = m_SavedInfo.Height;
            
            monitors = glfwGetMonitors(&nmonitors);
            for (i = 0; i < nmonitors; i++) {
                monitor_mode = glfwGetVideoMode(monitors[i]);
                glfwGetMonitorPos(monitors[i], &mx, &my);
                mw = monitor_mode->width;
                mh = monitor_mode->height;

                overlap =
                    std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
                    std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

                if (bestoverlap < overlap) {
                    bestoverlap = overlap;
                    bestmonitor = monitors[i];
                }
            }
            
            const GLFWvidmode * mode = glfwGetVideoMode(bestmonitor);

            // switch to full screen
            glfwSetWindowMonitor( m_WindowHandle, bestmonitor, 0, 0, mode->width, mode->height, 0 );
            
			m_Props.Fullscreen = true;
			m_Props.Maximized = false;
			m_Props.Minimized = false;
		}
	}

	void LinuxWindow::SetVSync(bool enabled)
	{
		MT_PROFILE_FUNCTION();
		
		if (m_RenderContext->SetVsync(enabled))
			m_Props.VSync = enabled;
		else
			MT_CORE_WARN("Failed to set vsync!");
	}

	inline bool LinuxWindow::IsKeyPressed(KeyCode code) const
	{
		if (code != KeyCode::Invalid)
			return m_Keys.at(code);
		else
			MT_CORE_WARN("Invalid key");

		return false;
	}
}