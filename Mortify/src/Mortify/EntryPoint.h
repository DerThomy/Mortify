#pragma once

#include "Mortify/Core/Core.h"
#include "Debug/Instrumentor.h"

int main(int argc, char** argv)
{
	Mortify::Log::Init();
	
	MT_PROFILE_BEGIN_SESSION("Startup", "MortifyProfile-Startup.json");
	auto app = Mortify::CreateApplication();
	MT_PROFILE_END_SESSION();

	MT_PROFILE_BEGIN_SESSION("Runtime", "MortifyProfile-Runtime.json");
	app->Run();
	MT_PROFILE_END_SESSION();

	MT_PROFILE_BEGIN_SESSION("Shutdown", "MortifyProfile-Shutdown.json");
	delete app;
	MT_PROFILE_END_SESSION();
}