#pragma once

#include <Windows.h>

#include "Mortify/Rendering/RenderContext.h"


namespace Mortify
{
	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);

	class WindowsGLRenderContext : public RenderContext
	{
	public:
		WindowsGLRenderContext(Window* window);

		void Init() override;
		void MakeContextCurrent() const override;
		void ReleaseContext() const override;
		procFunc GetProcFunc() override;
		void SwapBuffers() override;
		bool SetVsync(bool on) override;
		void Destroy() override;
		void* GetContextHandler() override { return (void*)m_OpenGLRenderContextHandler; };
		void SetContextHandler(void* handler) override { m_OpenGLRenderContextHandler = (HGLRC)handler; };
		void SaveContext() override { m_BackupHDC = m_DeviceContextHandler; };
		void RestoreContext() override { m_DeviceContextHandler = m_BackupHDC; };
		bool IsValid() override { return m_Valid; };

		static RenderContext::procAdr getGLProcAddress(const char* procname);

		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

	public:
		HWND m_WindowHandler;
		PIXELFORMATDESCRIPTOR m_PFD;
		HDC m_DeviceContextHandler;
		HGLRC m_OpenGLRenderContextHandler;

	private:
		HDC m_BackupHDC;
		bool m_Valid;
	};
}