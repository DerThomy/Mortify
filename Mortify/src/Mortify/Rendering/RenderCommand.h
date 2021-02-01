#pragma once

#include "Mortify/Rendering/RendererAPI.h"
#include "Mortify/Rendering/RenderContext.h"

#include <glm/glm.hpp>

namespace Mortify
{
	class RenderCommand
	{
	public:
		static inline void Init(const Ref<RenderContext>& context)
		{
			s_RenderAPI->Init(context);
		}

		static inline void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}

		static inline void Clear()
		{
			s_RenderAPI->Clear();
		}

		static inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			MT_CORE_INFO("Set Viewport to {0}, {1}", width, height);
			s_RenderAPI->SetViewport(x, y, width, height);
		}

		static inline void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}

	private:
		static Scope<RendererAPI> s_RenderAPI;
	};
}