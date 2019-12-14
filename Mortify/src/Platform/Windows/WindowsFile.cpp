#include "mtpch.h"

#include "Platform/Windows/WindowsFile.h"

#include <Windows.h>

namespace Mortify
{
	WindowsPath::WindowsPath(const std::string& path)
	{
		
	}

	WindowsFile::WindowsFile(const std::string& path)
		: m_Path(path)
	{
		std::string pathStr = m_Path.GetRelativePath();
		std::string ext;
		size_t dotPos = pathStr.rfind('.');
		MT_CORE_ASSERT(dotPos != std::string::npos, "Path is no FilePath");
		m_Extension = dotPos == std::string::npos ? "" : pathStr.substr(dotPos, pathStr.back() - dotPos);
	}

	std::vector<std::string> WindowsPath::GetReservedFileNames() const
	{
		return {
			"CON", "PRN", "AUX", "NUL", "COM1", "COM2", "COM3", "COM4", "COM5", "COM6",
			"COM7", "COM8", "COM9", "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
		};
	}

	bool WindowsPath::IsDirectoryPath() const
	{
		return m_Path.find(".") == std::string::npos;
	}

	bool WindowsPath::IsValid() const
	{
		for (auto chr : GetInvalidChars())
			if (m_Path.find(chr) != std::string::npos)
				return false;

		for (auto name : GetReservedFileNames())
			if (m_Path == name)
				return false;

		return true;
	}

	std::string WindowsPath::GetWorkingDirectory() const
	{
		char buf[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, buf);
		return std::string(buf);
	}

	Path::PathType WindowsPath::GetPathTypeFromString(const std::string& path)
	{
		return Path::PathType::None;
	}

	const std::string& WindowsPath::RelativeToAbsolutePath(const std::string& rel_path) const
	{
		std::string workDir = GetWorkingDirectory();
		return workDir + rel_path;
	}

	const std::string& WindowsPath::AbsoluteToRelativePath(const std::string& abs_path) const
	{
		std::string workDir = GetWorkingDirectory();
		size_t offset = workDir.size() - 1;
		if (abs_path.substr(0, offset) == workDir)
			return abs_path.substr(offset, abs_path.back() - offset);

		return std::string();
	}
}
