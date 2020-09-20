#include "mtpch.h"

#include "LinuxGLRenderContext.h"
#include "Mortify/Core/Window.h"

namespace Mortify
{
	LinuxGLRenderContext::LinuxGLRenderContext(Window* window)
        : m_WindowHandle(static_cast<GLFWwindow*>(window->GetNativeWindow()))
	{
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MT_CORE_ASSERT(status, "Failed to initialize Glad!");

		MT_CORE_INFO("OpenGL Info:");
		MT_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		MT_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		MT_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		MT_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
		m_Valid = true;
	}

	void LinuxGLRenderContext::MakeContextCurrent() const
	{
		if (m_Valid)
		{
			glfwMakeContextCurrent(m_WindowHandle);
		}
	}

	void LinuxGLRenderContext::ReleaseContext() const
	{
		
	}

	RenderContext::procAdr LinuxGLRenderContext::getGLProcAddress(const char* procname)
	{
		const RenderContext::procAdr proc = (RenderContext::procAdr)glfwGetProcAddress(procname);

		if (proc)
			return proc;
        else
            MT_CORE_ASSERT(false, "Can't find procaddress for " + std::string(procname))
	}

	RenderContext::procFunc LinuxGLRenderContext::GetProcFunc()
	{
		return getGLProcAddress;
	}

	void LinuxGLRenderContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

	bool LinuxGLRenderContext::SetVsync(bool on)
	{
		glfwSwapInterval(on ? 1 : 0);
        return true;
	}

	void LinuxGLRenderContext::Destroy()
	{
		
	}
}