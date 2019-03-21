#pragma once

#include "Mortify/Rendering/Color.h"

namespace Mortify
{
	class RenderContext
	{
	public:
		virtual void Init() = 0;
		virtual void ClearBuffer(const Color& color) = 0;
		virtual void Present() = 0;
	};
}