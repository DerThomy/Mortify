#pragma once

#ifdef MT_PLATFORM_WINDOWS

extern Mortify::Application* Mortify::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Mortify::CreateApplication();
	app->Run();
	delete app;
}

#endif