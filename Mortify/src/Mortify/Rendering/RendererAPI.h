#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Mortify
{
	using RendererID = uint32_t;

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL
		};

	public:
		virtual void Init() = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void SetViewport(float width, float height) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		static inline API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}