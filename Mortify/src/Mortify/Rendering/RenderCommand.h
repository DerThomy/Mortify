#pragma once

#include "RendererAPI.h"

namespace Mortify
{
	class RenderCommand
	{
	public:
		static inline void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}

		static inline void Clear()
		{
			s_RenderAPI->Clear();
		}

		static inline void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RenderAPI;
	};
}