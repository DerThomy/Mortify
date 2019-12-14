#include "mtpch.h"

#include "Mortify/Rendering/VertexArray.h"
#include "Mortify/Rendering/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mortify
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: MT_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}
}