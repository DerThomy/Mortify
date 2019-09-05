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
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static inline const ResourceManager const* GetResourceManager() { return m_ResourceManager; }
	private:
		struct SceneData
		{
			uint32_t SceneID;
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
		static ResourceManager* m_ResourceManager;
		static uint32_t m_SceneCounter;
	};
}