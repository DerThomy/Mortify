#include "mtpch.h"

#include "Mortify/Rendering/RenderCommand.h"

namespace Mortify
{
	Scope<RendererAPI> RenderCommand::s_RenderAPI = RendererAPI::Create();
}