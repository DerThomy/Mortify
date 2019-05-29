#pragma once

namespace Mortify
{
	class RenderContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};
}