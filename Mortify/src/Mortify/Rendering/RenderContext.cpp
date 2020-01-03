#include "mtpch.h"
#include "Mortify/Rendering/RenderContext.h"

#include "Mortify/Rendering/Renderer.h"
#include "Mortify/Core/Window.h"

#include "Platform/Windows/WindowsRenderContext.h"

namespace Mortify
{
	Ref<RenderContext> RenderContext::Create(Window* window)
	{
		Ref<RenderContext> context = Ref<RenderContext>();

	#ifdef MT_PLATFORM_WINDOWS
		context = WindowsRenderContext::Create(window);
	#endif

		MT_CORE_ASSERT(context, "Platform not supported!");
		return context;
	}
}