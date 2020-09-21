#include "mtpch.h"

#include "Mortify/Rendering/Renderer.h"
#include "Mortify/Rendering/Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Mortify
{
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::Nil: MT_CORE_ASSERT(false, "RenderAPI::Nil is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::Nil: MT_CORE_ASSERT(false, "RenderAPI::Nil is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}
}