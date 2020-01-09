#include "mtpch.h"

#include "WindowsOS.h"

#include <timeapi.h>

namespace Mortify
{
	WindowsOS::WindowsOS()
	{
		WindowsOS::Init();
	}

	void WindowsOS::Init()
	{
		LoadLibraries();
		InitTimer();
	}

	double WindowsOS::GetTime() const
	{
		uint64_t value;

		if (m_hasPC)
			QueryPerformanceCounter((LARGE_INTEGER*) &value);
		else
			value = (uint64_t)timeGetTime();

		return (double)(value - m_Offset) / m_Frequency;
	}

	std::wstring WindowsOS::WideCharFromUTF8(const std::string& utf8_string) const
	{
		if (utf8_string.empty())
		{
			return std::wstring();
		}
		int count = MultiByteToWideChar( CP_UTF8 , MB_ERR_INVALID_CHARS , utf8_string.c_str() , -1, NULL , 0 );
		MT_CORE_ASSERT(count, "Failed to count ut8_string");
		std::wstring wstr;
		wstr.resize(count);
		MT_CORE_ASSERT(MultiByteToWideChar(CP_UTF8 , MB_ERR_INVALID_CHARS , utf8_string.c_str() , utf8_string.length(), &wstr[0] , count )
			, "Failed to convert utf8 string to widestring");
		return wstr;
	}

	BOOL WindowsOS::IsWindowsXPOrGreater()
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINXP), LOBYTE(_WIN32_WINNT_WINXP), 0);
	}

	BOOL WindowsOS::IsWindowsVistaOrGreater()
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_VISTA), LOBYTE(_WIN32_WINNT_VISTA), 0);
	}

	BOOL WindowsOS::IsWindows7OrGreater()
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN7), LOBYTE(_WIN32_WINNT_WIN7), 0);
	}

	BOOL WindowsOS::IsWindows8OrGreater()
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN8), LOBYTE(_WIN32_WINNT_WIN8), 0);
	}

	BOOL WindowsOS::IsWindows8Point1OrGreater()
	{
		return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINBLUE), LOBYTE(_WIN32_WINNT_WINBLUE), 0);
	}

	BOOL WindowsOS::IsWindows10AnniversaryUpdateOrGreater()
	{
		return IsWindows10BuildOrGreater(14393);
	}

	BOOL WindowsOS::IsWindows10CreatorsUpdateOrGreater()
	{
		return IsWindows10BuildOrGreater(15063);
	}

	void WindowsOS::InitTimer()
	{
		uint64_t frequency;
		
		if (QueryPerformanceFrequency((LARGE_INTEGER*) &frequency))
		{
			m_hasPC = true;
			m_Frequency = frequency;
			QueryPerformanceCounter((LARGE_INTEGER*) &m_Offset);
		}
		else
		{
			m_hasPC = false;
			m_Frequency = 1000.0;
			m_Offset = (uint64_t)timeGetTime();
		}
	}

	void WindowsOS::LoadLibraries()
	{
		// Ntdll
		m_Libraries.Ntdll.instance = LoadLibraryA("ntdll.dll");
		MT_CORE_ASSERT(m_Libraries.Ntdll.instance, "Failed to load ntdll");
		m_Libraries.Ntdll.RtlVerifyVersionInfo = (PFN_RtlVerifyVersionInfo)
			GetProcAddress(m_Libraries.Ntdll.instance,"RtlVerifyVersionInfo");
	}

	BOOL WindowsOS::IsWindowsVersionOrGreater(WORD major, WORD minor, WORD sp)
	{
		OSVERSIONINFOEXW osinfo = {sizeof(osinfo), major, minor, 0, 0, {0}, sp};
		DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR;
		ULONGLONG condition = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
		condition = VerSetConditionMask(condition, VER_MINORVERSION, VER_GREATER_EQUAL);
		condition = VerSetConditionMask(condition, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);

		return m_Libraries.Ntdll.RtlVerifyVersionInfo(&osinfo, mask, condition) == 0;
	}

	BOOL WindowsOS::IsWindows10BuildOrGreater(WORD build)
	{
		OSVERSIONINFOEXW osinfo = {sizeof(osinfo), 10, 0, build};
		DWORD mask = VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER;
		ULONGLONG condition = VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
		condition = VerSetConditionMask(condition, VER_MINORVERSION, VER_GREATER_EQUAL);
		condition = VerSetConditionMask(condition, VER_BUILDNUMBER, VER_GREATER_EQUAL);

		return m_Libraries.Ntdll.RtlVerifyVersionInfo(&osinfo, mask, condition) == 0;
	}
}

