#include "mtpch.h"

#include "OS.h"
#include "Platform/Windows/WindowsOS.h"

namespace Mortify
{
	Ref<OS>& OS::s_Instance = OS::Create();
}