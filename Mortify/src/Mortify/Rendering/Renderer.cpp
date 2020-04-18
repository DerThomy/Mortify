#include "mtpch.h"

#include "Mortify/Rendering/Renderer.h"
#include "Mortify/Rendering/Renderer2D.h"

namespace Mortify
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init(const Ref<RenderContext>& context)
	{
		MT_PROFILE_FUNCTION();
		
		RenderCommand::Init(context);
		Renderer2D::Init(context);
	}

	void Renderer::BeginScene(OrthographicCamera& camera, const Ref<RenderContext> context)
	{
		context->MakeContextCurrent();
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(WindowID id, uint32_t width, uint32_t height)
	{
		auto window = WindowManager::GetWindowByID(id);
		if (window.has_value())
		{
			window.value()->GetContext()->MakeContextCurrent();
			RenderCommand::SetViewport(0, 0, width, height);
		}
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}
}