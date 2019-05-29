#include "mtpch.h"

#include "OpenGLRenderContext.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <gl/GL.h>

namespace Mortify
{
	OpenGLRenderContext::OpenGLRenderContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		MT_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLRenderContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MT_CORE_ASSERT(status, "Failed to initizalize Glad!");

		MT_CORE_INFO("OpenGL Info:");
		MT_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		MT_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		MT_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLRenderContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}