#include "mtpch.h"

#include "File.h"

#include "Platform/Windows/WindowsFile.h"

namespace Mortify
{
	Ref<File> File::Create(const std::string& path)
	{
		return std::make_shared<WindowsFile>(path);
	}
}