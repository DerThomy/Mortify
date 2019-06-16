#include "mtpch.h"

#include "Application.h"

#include "Mortify/Log.h"

#include "Mortify/Input.h"

#include "Mortify/Rendering/Renderer.h"


namespace Mortify
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MT_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		// Triangle rendering

		m_TriangleVA.reset(VertexArray::Create());

		float triangleVertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> triangleVB;
		triangleVB.reset(VertexBuffer::Create(triangleVertices, sizeof(triangleVertices)));

		BufferLayout triangleLayout = {
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};

		triangleVB->SetLayout(triangleLayout);

		m_TriangleVA->AddVertexBuffer(triangleVB);

		unsigned int triangleIndicies[3] = { 0, 1, 2 };

		std::shared_ptr<IndexBuffer> triangleIB;
		triangleIB.reset(IndexBuffer::Create(triangleIndicies, sizeof(triangleIndicies) / sizeof(uint32_t)));

		m_TriangleVA->SetIndexBuffer(triangleIB);

		// Square rendering

		m_SquareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
				{ShaderDataType::Float3, "a_Position"}
			}
		);

		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndicies[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string triangleVertexSource = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
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

		m_TriangleShader.reset(new Shader(triangleVertexSource, triangleFragmentSource));
		m_SquareShader.reset(new Shader(squareVertexSource, squareFragmentSource));
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()	
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_SquareShader->Bind();
			Renderer::Submit(m_SquareVA);

			m_TriangleShader->Bind();
			Renderer::Submit(m_TriangleVA);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
	
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}