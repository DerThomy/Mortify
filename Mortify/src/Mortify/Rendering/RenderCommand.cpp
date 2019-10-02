#include "mtpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mortify
{
	Scope<RendererAPI> RenderCommand::s_RenderAPI = CreateScope<OpenGLRendererAPI>();
}