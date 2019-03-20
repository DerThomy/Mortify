#pragma once

#include "Mortify/Rendering/Color.h"

namespace Mortify
{
	class RenderContext
	{
	public:
		virtual bool Init() = 0;
		virtual void Clear(Color& color) = 0;
		virtual void Present() = 0;
	};
}