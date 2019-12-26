#pragma once

#include "Mortify/Core/Window.h"

namespace Mortify
{
	class RenderContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;

		static Scope<RenderContext> Create(Window* window);
	};
}