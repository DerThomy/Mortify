#include <Mortify.h>
#include <Mortify/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Mortify::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D(Application::GetWindow(), MT_BIND_EVENT_FN(Application::OnEvent)));
	}

	~Sandbox()
	{}
};


Mortify::Application* Mortify::CreateApplication()
{
	return new Sandbox();
}