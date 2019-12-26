#include "mtpch.h"
#include "Mortify/Rendering/RenderContext.h"

#include "Mortify/Rendering/Renderer.h"
#include "Platform/OpenGL/OpenGLRenderContext.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Mortify
{
	Scope<RenderContext> RenderContext::Create(Window* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    MT_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRenderContext>(window);
		}

		MT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}