#include "mtpch.h"

#include "Mortify/Rendering/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mortify
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::Nil:    MT_CORE_ASSERT(false, "RendererAPI::Nil is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		MT_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}