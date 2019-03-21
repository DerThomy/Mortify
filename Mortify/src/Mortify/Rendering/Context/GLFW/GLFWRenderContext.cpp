#include "mtpch.h"
#include "GLFWRenderContext.h"

namespace Mortify
{
	void GLFWRenderContext::Init()
	{
		glfwMakeContextCurrent(m_Window);
	}

	void GLFWRenderContext::ClearBuffer(const Color& color)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
	}

	void GLFWRenderContext::Present()
	{
		glfwSwapBuffers(m_Window);
	}
}