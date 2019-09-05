#include <Mortify.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Mortify::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
	{
		m_TriangleVA.reset(Mortify::VertexArray::Create());

		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		Mortify::Ref<Mortify::VertexBuffer> triangleVB;
		triangleVB.reset(Mortify::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));

		Mortify::BufferLayout triangleLayout = {
			{Mortify::ShaderDataType::Float3, "a_Position"},
			{Mortify::ShaderDataType::Float4, "a_Color"}
		};

		triangleVB->SetLayout(triangleLayout);

		m_TriangleVA->AddVertexBuffer(triangleVB);

		unsigned int triangleIndicies[3] = { 0, 1, 2 };

		Mortify::Ref<Mortify::IndexBuffer> triangleIB;
		triangleIB.reset(Mortify::IndexBuffer::Create(triangleIndicies, sizeof(triangleIndicies) / sizeof(uint32_t)));

		m_TriangleVA->SetIndexBuffer(triangleIB);

		// Square rendering

		m_SquareVA.reset(Mortify::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		Mortify::Ref<Mortify::VertexBuffer> squareVB;
		squareVB.reset(Mortify::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
				{Mortify::ShaderDataType::Float3, "a_Position"}
			}
		);

		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };

		Mortify::Ref<Mortify::IndexBuffer> squareIB;
		squareIB.reset(Mortify::IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string triangleVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string triangleFragmentSource = R"(
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

		std::string squareVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSource = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_TriangleShader.reset(Mortify::Shader::Create(triangleVertexSource, triangleFragmentSource));
		m_SquareShader.reset(Mortify::Shader::Create(squareVertexSource, squareFragmentSource));
	}

	void OnUpdate(Mortify::Timestep ts) override
	{
		//MT_TRACE("Timestep for each frame: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

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
		else if (Mortify::Input::IsKeyPressed(MT_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		if (Mortify::Input::IsKeyPressed(MT_KEY_J))
			m_SquarePosition.x -= m_TransformSpeed * ts;
		else if (Mortify::Input::IsKeyPressed(MT_KEY_L))
			m_SquarePosition.x += m_TransformSpeed * ts;

		if (Mortify::Input::IsKeyPressed(MT_KEY_I))
			m_SquarePosition.y += m_TransformSpeed * ts;
		else if (Mortify::Input::IsKeyPressed(MT_KEY_K))
			m_SquarePosition.y -= m_TransformSpeed * ts;

		Mortify::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mortify::RenderCommand::Clear();

		Mortify::Renderer::BeginScene(m_Camera);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		std::dynamic_pointer_cast<Mortify::OpenGLShader>(m_SquareShader)->Bind();
		Mortify::Renderer::Submit(m_SquareVA, m_SquareShader, glm::translate(glm::mat4(1.0f), m_SquarePosition));

		std::dynamic_pointer_cast<Mortify::OpenGLShader>(m_TriangleShader)->Bind();
		Mortify::Renderer::Submit(m_TriangleVA, m_TriangleShader);

		Mortify::Renderer::EndScene();
	}

	void OnEvent(Mortify::Event& event) override
	{
		
	}

	virtual void OnImGuiRender() override
	{

	}

private:
	Mortify::Ref<Mortify::VertexArray> m_TriangleVA;
	Mortify::Ref<Mortify::Shader> m_TriangleShader;

	Mortify::Ref<Mortify::VertexArray> m_SquareVA;
	Mortify::Ref<Mortify::Shader> m_SquareShader;
	Mortify::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 3.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 20.0f;

	glm::vec3 m_SquarePosition;
	float m_TransformSpeed = 4.0f;
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