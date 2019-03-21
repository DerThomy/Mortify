#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mortify/Window.h"
#include "Mortify/Rendering/Context/RenderContext.h"

namespace Mortify
{
	class GLFWRenderContext : public RenderContext
	{
	private:
		GLFWwindow* m_Window;
	public:
		GLFWRenderContext(GLFWwindow* window)
			: m_Window(window) {}

		virtual void Init() override;
		virtual void ClearBuffer(const Color& color) override;
		virtual void Present() override;
	};
}