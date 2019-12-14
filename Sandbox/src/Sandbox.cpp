#include <Mortify.h>
#include <Mortify/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Mortify::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{}
};


Mortify::Application* Mortify::CreateApplication()
{
	return new Sandbox();
}