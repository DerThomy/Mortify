#include "mtpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mortify
{
	RendererAPI* RenderCommand::s_RenderAPI = new OpenGLRendererAPI;
}