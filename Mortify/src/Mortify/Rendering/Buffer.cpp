#include "mtpch.h"

#include "Renderer.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Mortify
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None: MT_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None: MT_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}
}