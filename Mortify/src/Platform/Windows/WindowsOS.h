#pragma once

#include "Mortify/Core/OS.h"
#include "Platform/Windows/WinodowsCore.h"

namespace Mortify
{	
	class WindowsOS : public OS
	{
	public:
		struct Libraries
		{
			USER32 User32;
			SHCORE Shcore;
			NTDLL Ntdll;
		};
		
		WindowsOS();
		~WindowsOS();

		void Init();
		
		double GetTime() const override;
		std::wstring WideCharFromUTF8(const std::string& utf8_string) const override;
		std::string UTF8FromWideString(const std::wstring& wide_string) const override;

		size_t GetCacheLineSize() const override;

		const Libraries& GetLibraries() const { return m_Libraries; };

		BOOL IsWindowsXPOrGreater();
		BOOL IsWindowsVistaOrGreater();
		BOOL IsWindows7OrGreater();
		BOOL IsWindows8OrGreater();
		BOOL IsWindows8Point1OrGreater();
		BOOL IsWindows10AnniversaryUpdateOrGreater();
		BOOL IsWindows10CreatorsUpdateOrGreater();

	private:
		void InitTimer();
		void LoadLibraries();
		
		BOOL IsWindowsVersionOrGreater(WORD major, WORD minor, WORD sp);
		BOOL IsWindows10BuildOrGreater(WORD build);
		
	private:		
		uint64_t	m_Offset;
		uint64_t	m_Frequency;
		bool		m_hasPC;

		Libraries m_Libraries;
	};
}