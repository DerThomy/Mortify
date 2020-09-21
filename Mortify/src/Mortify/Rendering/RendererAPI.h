#pragma once

#include "Mortify/Rendering/VertexArray.h"
#include "Mortify/Rendering/RenderContext.h"

#include <glm/glm.hpp>

namespace Mortify
{
	using RendererID = uint32_t;

	class RendererAPI
	{
	public:
		enum class API
		{
			Nil = 0, OpenGL
		};

	public:
		virtual void Init(const Ref<RenderContext>& context) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		static inline API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();

	private:
		static API s_API;
	};
}