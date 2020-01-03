#include "mtpch.h"

#include "Mortify/Rendering/Renderer.h"
#include "WindowsRenderContext.h"
#include "Mortify/Core/Window.h"

#include "RenderContexts/WindowsGLRenderContext.h"

namespace Mortify
{
	Ref<RenderContext> WindowsRenderContext::Create(Window* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    MT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<WindowsGLRenderContext>(window);
		}

		MT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}