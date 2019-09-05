#pragma once

#include "Mortify/Rendering/RendererAPI.h"

namespace Mortify
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void SetViewport(float width, float height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}