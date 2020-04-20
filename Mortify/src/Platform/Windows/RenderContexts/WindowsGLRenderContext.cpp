#include "mtpch.h"

#include "WindowsGLRenderContext.h"
#include "Mortify/Core/Window.h"

#include <glad/glad.h>

namespace Mortify
{
	static HMODULE openGLModule = LoadLibraryA("opengl32.dll");

	WindowsGLRenderContext::WindowsGLRenderContext(Window* window)
		: m_WindowHandler((HWND)window->GetNativeWindow()), m_BackupHDC(NULL)
	{
		m_DeviceContextHandler = GetDC(m_WindowHandler);
		m_PFD = {
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
			PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
			32,                   // Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			24,                   // Number of bits for the depthbuffer
			8,                    // Number of bits for the stencilbuffer
			0,                    // Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		MT_CORE_ASSERT(SetPixelFormat(m_DeviceContextHandler, ChoosePixelFormat(m_DeviceContextHandler, &m_PFD), &m_PFD),
			"Failed to set Pixel format");

		DescribePixelFormat(m_DeviceContextHandler, GetPixelFormat(m_DeviceContextHandler), sizeof(m_PFD), &m_PFD);

		m_OpenGLRenderContextHandler = wglCreateContext(m_DeviceContextHandler);

		MT_CORE_ASSERT(m_OpenGLRenderContextHandler, "Failed to create wgl context");
	}

	void WindowsGLRenderContext::Init()
	{
		MT_PROFILE_FUNCTION();

		MakeContextCurrent();
		int status = gladLoadGLLoader((GLADloadproc)getGLProcAddress);
		MT_CORE_ASSERT(status, "Failed to initizalize Glad!");

		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		MT_CORE_INFO("OpenGL Info:");
		MT_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		MT_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		MT_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

#ifdef MT_DEBUG
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		MT_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Mortify requires at least OpenGL version 4.5!");
#endif

		m_Valid = true;
	}

	void WindowsGLRenderContext::MakeContextCurrent() const
	{
		if (m_Valid)
		{
			MT_CORE_ASSERT(wglMakeCurrent(m_DeviceContextHandler, m_OpenGLRenderContextHandler),
				"Failed to make context current");
		}
	}

	void WindowsGLRenderContext::ReleaseContext() const
	{
		MT_CORE_ASSERT(wglMakeCurrent(m_DeviceContextHandler, nullptr),
			"Failed to release context");
	}

	RenderContext::procAdr WindowsGLRenderContext::getGLProcAddress(const char* procname)
	{
		const RenderContext::procAdr proc = (RenderContext::procAdr)wglGetProcAddress(procname);

		if (proc)
			return proc;

		return (RenderContext::procAdr)GetProcAddress(openGLModule, procname);
	}

	RenderContext::procFunc WindowsGLRenderContext::GetProcFunc()
	{
		return getGLProcAddress;
	}

	void WindowsGLRenderContext::SwapBuffers()
	{
		::SwapBuffers(m_DeviceContextHandler);
	}

	bool WindowsGLRenderContext::SetVsync(bool on)
	{
		if (wglSwapIntervalEXT != nullptr)
			return wglSwapIntervalEXT(1 ? on : 0);
		return false;
	}

	void WindowsGLRenderContext::Destroy()
	{
		m_Valid = false;
		wglMakeCurrent(m_DeviceContextHandler, NULL);
		wglDeleteContext(m_OpenGLRenderContextHandler);
	}
}