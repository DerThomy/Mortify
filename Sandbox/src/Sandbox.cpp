#include <Mortify.h>

#include "imgui.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class ExampleLayer : public Mortify::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//MT_INFO("ExampleLayer::Update");
	}

	void OnEvent(Mortify::Event& event) override
	{
		//MT_TRACE("{0}", event);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world 2");
		ImGui::Button("test", ImVec2(20, 10));
		ImGui::End();
	}
};

class Sandbox : public Mortify::Application
{
public:
	Sandbox()
	{
		m_Camera->SetLookAt({ 0, 0, 0 });
		m_Camera->SetPosition({ 0, 0, 2.0f });

		MT_INFO("Creating test imgui layer");
		PushLayer(new ExampleLayer());

		m_TriangleVA.reset(Mortify::VertexArray::Create());

		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		std::shared_ptr<Mortify::VertexBuffer> triangleVB;
		triangleVB.reset(Mortify::VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));

		Mortify::BufferLayout triangleLayout = {
			{Mortify::ShaderDataType::Float3, "a_Position"},
			{Mortify::ShaderDataType::Float4, "a_Color"}
		};

		triangleVB->SetLayout(triangleLayout);

		m_TriangleVA->AddVertexBuffer(triangleVB);

		unsigned int triangleIndicies[3] = { 0, 1, 2 };

		std::shared_ptr<Mortify::IndexBuffer> triangleIB;
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

		std::shared_ptr<Mortify::VertexBuffer> squareVB;
		squareVB.reset(Mortify::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
				{Mortify::ShaderDataType::Float3, "a_Position"}
			}
		);

		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<Mortify::IndexBuffer> squareIB;
		squareIB.reset(Mortify::IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string triangleVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 mvpMatrix;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = mvpMatrix * vec4(a_Position, 1.0);
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

			uniform mat4 mvpMatrix;

			void main()
			{
				v_Position = a_Position;
				gl_Position = mvpMatrix * vec4(a_Position, 1.0);
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

		m_TriangleShader.reset(new Mortify::Shader(triangleVertexSource, triangleFragmentSource));
		m_SquareShader.reset(new Mortify::Shader(squareVertexSource, squareFragmentSource));

		mvp_square_location = glGetUniformLocation(m_SquareShader->getProgramID(), "mvpMatrix");
		mvp_triangle_location = glGetUniformLocation(m_TriangleShader->getProgramID(), "mvpMatrix");
	}

	~Sandbox()
	{}

	virtual void RunImpl() override 
	{
		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 mvp = m_Camera->GetVP() * model;

		Mortify::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mortify::RenderCommand::Clear();

		Mortify::Renderer::BeginScene();

		m_SquareShader->Bind();
		Mortify::Renderer::Submit(m_SquareVA);

		glUniformMatrix4fv(mvp_square_location, 1, GL_FALSE, glm::value_ptr(mvp));

		m_TriangleShader->Bind();
		Mortify::Renderer::Submit(m_TriangleVA);

		glUniformMatrix4fv(mvp_triangle_location, 1, GL_FALSE, glm::value_ptr(mvp));

		Mortify::Renderer::EndScene();

		m_Camera->Update();
	}

private:
	std::shared_ptr<Mortify::VertexArray> m_TriangleVA;
	std::shared_ptr<Mortify::Shader> m_TriangleShader;

	std::shared_ptr<Mortify::VertexArray> m_SquareVA;
	std::shared_ptr<Mortify::Shader> m_SquareShader;

	GLint mvp_square_location;
	GLint mvp_triangle_location;
};


Mortify::Application* Mortify::CreateApplication()
{
	return new Sandbox();
}