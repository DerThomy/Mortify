#pragma once

#include "Mortify/Core/Core.h"

#ifdef MT_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
	Mortify::Log::Init();
	MT_CORE_WARN("Initialized Log");

	auto app = Mortify::CreateApplication();
	app->Run();
	delete app;
}

#endif