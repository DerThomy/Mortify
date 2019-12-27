#pragma once

#include "WindowsRenderContext.h"

namespace Mortify
{
	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);

	class WindowsGLRenderContext : public WindowsRenderContext
	{
	public:
		WindowsGLRenderContext(Window* window);

		virtual void Init() override;
		virtual void MakeContextCurrent() override;
		virtual procFunc GetProcFunc() override;
		virtual void SwapBuffers() override;
		virtual bool SetVsync(bool on) override;
		virtual void Destroy() override;
		virtual void* GetContextHandler() override { return (void*)m_OpenGLRenderContextHandler; };
		virtual void SetContextHandler(void* handler) override { m_OpenGLRenderContextHandler = (HGLRC)handler; };
		virtual void SaveContext() override { m_BackupHDC = m_DeviceContextHandler; };
		virtual void RestoreContext() override { m_DeviceContextHandler = m_BackupHDC; };

		static RenderContext::procAdr getGLProcAddress(const char* procname);

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

	public:
		HWND m_WindowHandler;
		PIXELFORMATDESCRIPTOR m_PFD;
		HDC m_DeviceContextHandler;
		HGLRC m_OpenGLRenderContextHandler;

	private:
		HDC m_BackupHDC;
	};
}