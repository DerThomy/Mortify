#include <Mortify.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Mortify::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray = Mortify::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Mortify::Ref<Mortify::VertexBuffer> vertexBuffer;
		vertexBuffer = Mortify::VertexBuffer::Create(vertices, sizeof(vertices));
		Mortify::BufferLayout layout = {
			{ Mortify::ShaderDataType::Float3, "a_Position" },
			{ Mortify::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Mortify::Ref<Mortify::IndexBuffer> indexBuffer;
		indexBuffer = Mortify::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Mortify::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Mortify::Ref<Mortify::VertexBuffer> squareVB;
		squareVB = Mortify::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Mortify::ShaderDataType::Float3, "a_Position" },
			{ Mortify::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Mortify::Ref<Mortify::IndexBuffer> squareIB;
		squareIB = Mortify::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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

		std::dynamic_pointer_cast<Mortify::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Mortify::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Mortify::Timestep ts) override
	{
		if (Mortify::Input::IsKeyPressed(MT_KEY_LEFT) || Mortify::Input::IsKeyPressed(MT_KEY_A))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Mortify::Input::IsKeyPressed(MT_KEY_RIGHT) || Mortify::Input::IsKeyPressed(MT_KEY_D))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Mortify::Input::IsKeyPressed(MT_KEY_UP) || Mortify::Input::IsKeyPressed(MT_KEY_W))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Mortify::Input::IsKeyPressed(MT_KEY_DOWN) || Mortify::Input::IsKeyPressed(MT_KEY_S))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Mortify::Input::IsKeyPressed(MT_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Mortify::Input::IsKeyPressed(MT_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Mortify::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mortify::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Mortify::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Mortify::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Mortify::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

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

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Mortify::Event & event) override
	{
	}

private:
	Mortify::ShaderLibrary m_ShaderLibrary;
	Mortify::Ref<Mortify::Shader> m_Shader;
	Mortify::Ref<Mortify::VertexArray> m_VertexArray;

	Mortify::Ref<Mortify::Shader> m_FlatColorShader;
	Mortify::Ref<Mortify::VertexArray> m_SquareVA;

	Mortify::Ref<Mortify::Texture2D> m_Texture, m_ChernoLogoTexture;

	Mortify::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 80.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Mortify::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{}
};


Mortify::Application* Mortify::CreateApplication()
{
	return new Sandbox();
}