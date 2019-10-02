#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Camera.h"
#include "Material.h"
#include "Mortify/Core/Resource.h"

#include <glm/glm.hpp>

namespace Mortify
{
	class Renderer
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			uint32_t SceneID;
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
		static uint32_t m_SceneCounter;
	};

	class Renderer2D
	{
	public:
		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(float length, float height, Ref<Texture2D> texture, glm::vec3 color = { 0.0f, 0.0f, 0.0f }, const glm::mat4 & transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			uint32_t SceneID;
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
		static uint32_t m_SceneCounter;
	};
}