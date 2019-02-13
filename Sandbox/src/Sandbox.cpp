#include <Mortify.h>

class Sandbox : public Mortify::Application
{
public:
	Sandbox()
	{}

	~Sandbox()
	{}
};


Mortify::Application* Mortify::CreateApplication()
{
	return new Sandbox();
}