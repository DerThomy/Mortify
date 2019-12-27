#pragma once

namespace Mortify
{
	class Window;

	class RenderContext
	{
	public:
		typedef void (*procAdr) (void);
		typedef procAdr(*procFunc)(const char*);

		virtual void Init() = 0;
		virtual void MakeContextCurrent() = 0;
		virtual procFunc GetProcFunc() = 0;
		virtual void SwapBuffers() = 0;
		virtual bool SetVsync(bool on) = 0;
		virtual void Destroy() = 0;
		virtual void* GetContextHandler() = 0;
		virtual void SetContextHandler(void* handler) = 0;
		virtual void SaveContext() = 0;
		virtual void RestoreContext() = 0;

		static Ref<RenderContext> Create(Window* window);
	};
}