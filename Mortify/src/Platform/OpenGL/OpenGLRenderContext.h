#pragma once

#include "Mortify/Rendering/RenderContext.h"

struct GLFWwindow;

namespace Mortify
{
	class OpenGLRenderContext : public RenderContext
	{
	public:
		OpenGLRenderContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffer() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}