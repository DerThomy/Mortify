#pragma once

#include "Mortify/Rendering/Color.h"
#include "Mortify/Rendering/Context/RenderContext.h"

namespace Mortify
{
	class GLFWRenderContext : public RenderContext
	{
	public:
		virtual bool Init() override;
		virtual void Clear(Color& color) override;
		virtual void Present() override;
	};
}