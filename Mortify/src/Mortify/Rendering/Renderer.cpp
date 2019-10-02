#include "mtpch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Mortify
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMatrix4f("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMatrix4f("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	// 2D
	// ------------------------------------------------------------------------------------------------------------

	Scope<Renderer2D::SceneData> Renderer2D::s_SceneData = CreateScope<Renderer2D::SceneData>();

	void Renderer2D::Init()
	{
		RenderCommand::Init();
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(float length, float height, Ref<Texture2D> texture, glm::vec3 color, const glm::mat4& transform)
	{
		Ref<Shader> m_TextureShader = Shader::Create("../Sandbox/assets/shaders/Texture.glsl");

		std::dynamic_pointer_cast<OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(m_TextureShader)->UploadUniformMatrix4f("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(m_TextureShader)->UploadUniformMatrix4f("u_Transform", transform);

		if (texture)
			std::dynamic_pointer_cast<OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

		float halfLength = length / 2;
		float halfHeight = height / 2;

		float vertices[5 * 4] = {
			-halfLength,	-halfHeight,	0.0f, 0.0f, 0.0f,
			halfLength,		-halfHeight,	0.0f, 1.0f, 0.0f,
			halfLength,		halfHeight,		0.0f, 1.0f, 1.0f,
			-halfLength,	halfHeight,		0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> vBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		vBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float3, "a_TexCoord" }
		});

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

		Ref<IndexBuffer> iBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		Ref<VertexArray> vArray = VertexArray::Create();
		vArray->AddVertexBuffer(vBuffer);
		vArray->SetIndexBuffer(iBuffer);

		vArray->Bind();
		RenderCommand::DrawIndexed(vArray);
	}

}