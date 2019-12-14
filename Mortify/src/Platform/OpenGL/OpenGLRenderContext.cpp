#include "mtpch.h"

#include "Platform/OpenGL/OpenGLRenderContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

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

	#ifdef MT_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		MT_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
	#endif
	}

	void OpenGLRenderContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}