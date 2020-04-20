#include "mtpch.h"

#include "Mortify/Rendering/Renderer.h"
#include "Mortify/Rendering/Renderer2D.h"
#include "Mortify/Core/WindowManager.h"

namespace Mortify
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
	Ref<RenderContext> Renderer::s_CurrentContext {};

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
		auto& window = WindowManager::GetWindowByID(id);

		if (!window.has_value() || !window.value())
			return;

		auto& context = window.value()->GetContext();
		if (context && context->IsValid())
		{
			context->MakeContextCurrent();
			RenderCommand::SetViewport(0, 0, width, height);
		}
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	const Ref<RenderContext>& Renderer::GetCurrentContext()
	{
		return s_CurrentContext;
	}

	void Renderer::SetContext(const Ref<RenderContext>& context)
	{
		if (!context || !context->IsValid())
		{
			MT_CORE_ERROR("Invalid RenderContext");
			s_CurrentContext = nullptr;
			return;
		}

		context->MakeContextCurrent();
		s_CurrentContext = context;
	}

	void Renderer::RemoveContext(const Ref<RenderContext>& context)
	{
		if (s_CurrentContext == context)
			s_CurrentContext = nullptr;
	}
}