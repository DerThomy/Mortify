#pragma once

#include "RendererAPI.h"

#include <glm/glm.hpp>

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

		static inline void SetViewport(float width, float height)
		{
			s_RenderAPI->SetViewport(width, height);
		}

		static inline void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* s_RenderAPI;
	};
}