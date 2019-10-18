#include "mtpch.h"

#include "Renderer2D.h"
#include "RenderCommand.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Mortify
{
	struct Renderer2DStorage
	{
		Ref<Shader> FlatColorShader;
		Ref<VertexArray> QuadVertexArray;
	};

	static Renderer2DStorage* s_Data;
	
	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();
		
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		auto squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		const auto squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<Mortify::OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Mortify::OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMatrix4f("u_ViewProjection", camera.GetProjectionMatrix());
		std::dynamic_pointer_cast<Mortify::OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMatrix4f("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<Mortify::OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Mortify::OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}
