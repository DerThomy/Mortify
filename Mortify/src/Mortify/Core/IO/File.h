#pragma once

namespace Mortify
{
	class Path
	{
	public:
		enum class PathType
		{
			None = 0,
			Drive,
			Network
		};

		virtual const std::string& GetAbsolutePath() const = 0;

		virtual const std::string& GetRelativePath() const = 0;
			
		virtual PathType GetPathType() const = 0;

		virtual std::vector<char> GetDelimiter() const = 0;

		virtual std::vector<char> GetInvalidChars() const = 0;

		virtual std::vector<std::string> GetReservedFileNames() const = 0;

		virtual bool IsDirectoryPath() const = 0;

		virtual bool IsValid() const = 0;

	protected:
		virtual const std::string& RelativeToAbsolutePath(const std::string& rel_path) const = 0;

		virtual const std::string& AbsoluteToRelativePath(const std::string& abs_Path) const = 0;
	};

	class File
	{
	public:
		enum AccessFlags
		{
			None = 0,
			Read =	BIT(0),
			Write =	BIT(1),
			Exec =	BIT(2)
		};

		virtual const Path& GetPath() const = 0;

		virtual bool IsOpen() const = 0;

		virtual unsigned int GetSize() const = 0;

		virtual std::string GetName() const = 0;

		virtual AccessFlags GetAccessFlags() const = 0;

		virtual std::string GetExtension() const = 0;

		static Ref<File> Create(const std::string& path);
	};
}