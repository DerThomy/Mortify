#include "mtpch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Mortify
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();
	ResourceManager* Renderer::m_ResourceManager = new ResourceManager();

	uint32_t Renderer::m_SceneCounter = 0;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMatrix4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMatrix4f("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}