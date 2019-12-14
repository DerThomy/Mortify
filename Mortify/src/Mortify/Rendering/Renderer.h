#pragma once

#include "Mortify/Rendering/RendererAPI.h"
#include "Mortify/Rendering/RenderCommand.h"
#include "Mortify/Rendering/Camera.h"
#include "Mortify/Rendering/Material.h"
#include "Mortify/Core/Resource.h"

#include <glm/glm.hpp>

namespace Mortify
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		static void OnWindowResize(uint32_t width, uint32_t height);

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