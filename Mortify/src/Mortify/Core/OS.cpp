#include "mtpch.h"

#include "OS.h"
#include "Platform/Windows/WindowsOS.h"

namespace Mortify
{
	Ref<OS>& OS::s_Instance = Ref<OS>();

	Ref<OS> OS::Create()
	{
		MT_CORE_ASSERT(!s_Instance, "OS already exists");

	#ifdef MT_PLATFORM_WINDOWS
		s_Instance = CreateRef<WindowsOS>();
	#endif	
		return s_Instance;
	}
}