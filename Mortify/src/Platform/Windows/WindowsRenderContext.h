#pragma once

#include "Mortify/Rendering/RenderContext.h"

namespace Mortify
{
	class WindowsRenderContext : public RenderContext
	{
	public:
		static Ref<RenderContext> Create(Window* window);
	};
}