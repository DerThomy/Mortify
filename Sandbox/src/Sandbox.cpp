#include <Mortify.h>

class Sandbox : public Mortify::Application
{
public:
	Sandbox()
	{}

	~Sandbox()
	{}
};

int main(void)
{
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
}