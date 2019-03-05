#pragma once

#ifdef MT_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Mortify::Log::Init();
	MT_CORE_WARN("Initialized Log");
	MT_INFO("Hello Var={0}", 5);

	auto app = Mortify::CreateApplication();
	app->Run();
	delete app;
}

#endif