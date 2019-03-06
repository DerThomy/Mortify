#include <Mortify.h>

#include "imgui.h"

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
		ImGui::Text("Hello world");
		ImGui::End();
	}
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