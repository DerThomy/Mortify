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
		MT_INFO("Creating test imgui layer");
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{}
};


Mortify::Application* Mortify::CreateApplication()
{
	return new Sandbox();
}