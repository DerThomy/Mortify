#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Mortify/Rendering/RenderContext.h"


namespace Mortify
{
	class LinuxGLRenderContext : public RenderContext
	{
	public:
		LinuxGLRenderContext(Window* window);

		void Init() override;
		void MakeContextCurrent() const override;
		void ReleaseContext() const override;
		procFunc GetProcFunc() override;
		void SwapBuffers() override;
		bool SetVsync(bool on) override;
		void Destroy() override;
		void* GetContextHandler() override { return (void*)glfwGetGLXContext(m_WindowHandle); };
		bool IsValid() override { return m_Valid; };

		static RenderContext::procAdr getGLProcAddress(const char* procname);
        
    private:
        GLFWwindow* m_WindowHandle;
        bool m_Valid = false;
	};
}