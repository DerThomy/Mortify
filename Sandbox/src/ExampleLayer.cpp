#include "ExampleLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer(const Mortify::Ref<Mortify::Window>& window)
	: Layer("Example"), m_CameraController(window, true), m_Window(window)
{
	m_VertexArray = Mortify::VertexArray::Create();

	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	auto vertexBuffer = Mortify::VertexBuffer::Create(vertices, sizeof(vertices));
	Mortify::BufferLayout layout = {
		{ Mortify::ShaderDataType::Float3, "a_Position" },
		{ Mortify::ShaderDataType::Float4, "a_Color" }
	};
	vertexBuffer->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t indices[3] = { 0, 1, 2 };
	auto indexBuffer = Mortify::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	m_SquareVA = Mortify::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	auto squareVB = Mortify::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
	squareVB->SetLayout({
		{ Mortify::ShaderDataType::Float3, "a_Position" },
		{ Mortify::ShaderDataType::Float2, "a_TexCoord" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	auto squareIB = Mortify::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	std::string vertexSrc = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;
		out vec3 v_Position;
		out vec4 v_Color;
		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
		}
	)";

	std::string fragmentSrc = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;
		in vec3 v_Position;
		in vec4 v_Color;
		void main()
		{
			color = vec4(v_Position * 0.5 + 0.5, 1.0);
			color = v_Color;
		}
	)";

	m_Shader = Mortify::Shader::Create("Triangle Shader", vertexSrc, fragmentSrc);

	std::string flatColorShaderVertexSrc = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;
		out vec3 v_Position;
		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
		}
	)";

	std::string flatColorShaderFragmentSrc = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;
		in vec3 v_Position;
		
		uniform vec3 u_Color;
		void main()
		{
			color = vec4(u_Color, 1.0);
		}
	)";

	m_FlatColorShader = Mortify::Shader::Create("Flat Color", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
	auto m_TextureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = Mortify::Texture2D::Create("assets/textures/Checkerboard.png");
	m_ChernoLogoTexture = Mortify::Texture2D::Create("assets/textures/ChernoLogo.png");

	m_TextureShader->Bind();
	m_TextureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnUpdate(Mortify::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Mortify::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Mortify::RenderCommand::Clear();

	Mortify::Renderer::BeginScene(m_CameraController.GetCamera(), m_Window->GetContext());

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Mortify::Renderer::Submit(m_SquareVA, m_FlatColorShader, transform);
		}
	}

	m_Texture->Bind();
	Mortify::Renderer::Submit(m_SquareVA, m_ShaderLibrary.Get("Texture"), glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	m_ChernoLogoTexture->Bind();
	Mortify::Renderer::Submit(m_SquareVA, m_ShaderLibrary.Get("Texture"), glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	// Triangle
	// Mortify::Renderer::Submit(m_Shader, m_VertexArray);

	Mortify::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(Mortify::Event& event)
{
	m_CameraController.OnEvent(event);
}