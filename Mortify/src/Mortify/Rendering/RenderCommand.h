#pragma once

#include "Context/RenderContext.h"

namespace Mortify
{
	struct RenderCommand
	{
		virtual void Execute(RenderContext* context) = 0;
	};
}