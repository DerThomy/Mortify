#include "mtpch.h"

#include "Mortify/Rendering/RenderContext.h"
#include "Mortify/Rendering/Renderer.h"
#include "Mortify/Core/Window.h"

#include "RenderContexts/LinuxGLRenderContext.h"

namespace Mortify
{
	Ref<RenderContext> RenderContext::Create(Window* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::Nil:    MT_CORE_ASSERT(false, "RendererAPI::Nil is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<LinuxGLRenderContext>(window);
		}

		MT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}