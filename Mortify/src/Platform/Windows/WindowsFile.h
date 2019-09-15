#pragma once

#include "Mortify/Core/IO/File.h"

#include <Shlwapi.h>

namespace Mortify
{
	class WindowsPath : public Path
	{
	public:
		WindowsPath(const std::string& path);

		virtual const std::string& GetAbsolutePath() const override { return m_Absolute ? m_Path : RelativeToAbsolutePath(m_Path); }

		virtual const std::string& GetRelativePath() const override { return m_Absolute ? AbsoluteToRelativePath(m_Path) : m_Path; }

		virtual PathType GetPathType() const override { return m_PathType; }

		virtual std::vector<char> GetDelimiter() const override { return { '/', '\\' }; }

		virtual std::vector<char> GetInvalidChars() const override { return { '<', '>', ':', '"', '/', '\\', '|', '?', '*' }; }

		virtual std::vector<std::string> GetReservedFileNames() const override;

		virtual bool IsDirectoryPath() const override;

		virtual bool IsValid() const override;

	private:
		PathType GetPathTypeFromString(const std::string& path);

		const std::string& RelativeToAbsolutePath(const std::string& rel_path) const override;

		const std::string& AbsoluteToRelativePath(const std::string& abs_path) const override;

		// TODO: Auslagern der funktion in eigene statische OS klasse
		std::string GetWorkingDirectory() const;

	private:
		std::string m_Path;
		PathType m_PathType;
		bool m_Absolute;
	};

	class WindowsFile : public File
	{
	public:
		WindowsFile(const std::string& path);

		virtual const Path& GetPath() const override { return m_Path; }

		virtual bool IsOpen() const override;

		virtual unsigned int GetSize() const override;

		virtual std::string GetName() const override;

		virtual AccessFlags GetAccessFlags() const override;

		virtual std::string GetExtension() const override { return m_Extension; }

	private:
		std::string m_Extension;
		WindowsPath m_Path;
	};
}