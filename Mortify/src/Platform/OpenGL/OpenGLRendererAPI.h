#pragma once

#include "Mortify/Rendering/RendererAPI.h"
#include "Mortify/Rendering/RenderContext.h"

namespace Mortify
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init(const Ref<RenderContext>& context) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}