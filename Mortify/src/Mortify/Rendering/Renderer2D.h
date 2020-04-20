#pragma once

#include "Mortify/Rendering/Camera.h"
#include "Mortify/Rendering/Texture.h"
#include "Mortify/Rendering/Shader.h"
#include "Mortify/Rendering/VertexArray.h"
#include "Mortify/Rendering/RenderContext.h"

namespace Mortify
{	
	struct QuadProperties
	{
		glm::vec3 Position = {0.0f, 0.0f, 0.0f};
		glm::vec2 Size = {1.0f, 1.0f};
		float Rotation = 0.0f;
		glm::vec4 Color = {0.0f, 0.0f, 0.0f, 1.0f };
		Ref<Texture2D> Texture = nullptr;
		float TilingFactor = 1.0f;
	};

	class Renderer2D
	{
	public:
		static void Init(const Ref<RenderContext>& context);
		static void SetContext(const Ref<RenderContext>& context);
		static void RemoveContext(const Ref<RenderContext>& context);
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const QuadProperties& quad);

	private:
		static Ref<RenderContext> s_CurrentContext;
	};
}