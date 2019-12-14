#include "mtpch.h"

#include "Mortify/Rendering/Renderer2D.h"
#include "Mortify/Rendering/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Mortify
{
	struct Renderer2DStorage
	{
		Ref<Texture2D> WhiteTexture;
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
	};

	static Renderer2DStorage* s_Data;
	
	void Renderer2D::Init()
	{
		MT_PROFILE_FUNCTION();
		
		s_Data = new Renderer2DStorage();
		
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		auto squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		const auto squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		MT_PROFILE_FUNCTION();
		
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		MT_PROFILE_FUNCTION();
		
	}

	void Renderer2D::DrawQuad(const QuadProperties& quad)
	{
		MT_PROFILE_FUNCTION();
		
		s_Data->TextureShader->SetFloat4("u_Color", quad.Color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", quad.Texture == s_Data->WhiteTexture ? 1.0f : quad.TilingFactor);

		if (quad.Texture)
			quad.Texture->Bind();
		else
			s_Data->WhiteTexture->Bind();

		glm::mat4 transform;

		if (quad.Rotation != 0)
		{
			transform = glm::translate(glm::mat4(1.0f), quad.Position)
			* glm::rotate(glm::mat4(1.0f), quad.Rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });	
		}
		else
		{
			transform = glm::translate(glm::mat4(1.0f), quad.Position)
			* glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });
		}
		
		s_Data->TextureShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}
