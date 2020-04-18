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

	Ref<RenderContext> CurrentContext{};
	static std::unordered_map<Ref<RenderContext>, Renderer2DStorage*> s_Data;
	
	void Renderer2D::Init(const Ref<RenderContext>& context)
	{
		MT_PROFILE_FUNCTION();
		
		auto data = new Renderer2DStorage();

		context->MakeContextCurrent();
		
		data->QuadVertexArray = VertexArray::Create();

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
		data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		const auto squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		data->QuadVertexArray->SetIndexBuffer(squareIB);

		data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		data->TextureShader->Bind();
		data->TextureShader->SetInt("u_Texture", 0);

		s_Data[context] = data;
	}

	void Renderer2D::Shutdown()
	{
		for (auto& data : s_Data)
			delete data.second;

		s_Data.clear();
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera, const Ref<RenderContext>& context)
	{
		MT_PROFILE_FUNCTION();

		MT_CORE_ASSERT(s_Data.find(context) != s_Data.end(), "Renderer is not initialized for Context");

		auto& data = s_Data[context];
		
		context->MakeContextCurrent();
		CurrentContext = context;

		data->TextureShader->Bind();
		data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		MT_PROFILE_FUNCTION();
	}

	void Renderer2D::DrawQuad(const QuadProperties& quad)
	{
		MT_PROFILE_FUNCTION();

		MT_CORE_ASSERT(CurrentContext && s_Data.find(CurrentContext) != s_Data.end(),
			"Renderer is not initialized for Context");

		auto& data = s_Data[CurrentContext];
		data->TextureShader->SetFloat4("u_Color", quad.Color);
		data->TextureShader->SetFloat("u_TilingFactor", quad.Texture == data->WhiteTexture ? 1.0f : quad.TilingFactor);

		if (quad.Texture)
			quad.Texture->Bind();
		else
			data->WhiteTexture->Bind();

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
		
		data->TextureShader->SetMat4("u_Transform", transform);

		data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(data->QuadVertexArray);
	}
}
