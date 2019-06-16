#include "mtpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Mortify
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::None: MT_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RenderAPI::OpenGL: return new OpenGLVertexArray();
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}
}