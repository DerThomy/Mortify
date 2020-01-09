#pragma once

#include "Mortify/Core/OS.h"
#include <Windows.h>

// Typedefs Ntdll.dll
	typedef LONG (WINAPI * PFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*,ULONG,ULONGLONG);

namespace Mortify
{
	struct NTDLL
	{
		HMODULE instance;
		PFN_RtlVerifyVersionInfo RtlVerifyVersionInfo;
	};
	
	class WindowsOS : public OS
	{
	public:
		WindowsOS();

		void Init();
		
		double GetTime() const override;
		std::wstring WideCharFromUTF8(const std::string& utf8_string) const override;

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

		struct Libraries
		{
			NTDLL Ntdll;
		};

		Libraries m_Libraries;
	};
}