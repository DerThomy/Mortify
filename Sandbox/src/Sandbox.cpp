#include <Mortify.h>

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