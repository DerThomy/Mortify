#include "mtpch.h"

#include "WindowsOS.h"

#include <timeapi.h>

namespace Mortify
{
	WindowsOS::WindowsOS()
	{
		WindowsOS::Init();
	}

	WindowsOS::~WindowsOS()
	{
		FreeLibrary(m_Libraries.User32.Instance);
		FreeLibrary(m_Libraries.Shcore.Instance);
		FreeLibrary(m_Libraries.Ntdll.Instance);
	}

	Ref<OS> OS::Create() 
	{
		return CreateRef<WindowsOS>();
	}

	void WindowsOS::Init()
	{
		LoadLibraries();
		InitTimer();

		if(IsWindows10AnniversaryUpdateOrGreater())
			m_Libraries.User32.SetProcessDpiAwarnessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		else if (IsWindows8Point1OrGreater())
			m_Libraries.Shcore.SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		else if (IsWindowsVistaOrGreater())
			m_Libraries.User32.SetProcessDPIAware();
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
		size_t count = MultiByteToWideChar( CP_UTF8 , MB_ERR_INVALID_CHARS , utf8_string.c_str() , -1, NULL , 0 );
		MT_CORE_ASSERT(count, "Failed to count ut8_string");
		std::wstring wstr;
		wstr.resize(count);
		MT_CORE_ASSERT(MultiByteToWideChar(CP_UTF8 , MB_ERR_INVALID_CHARS , utf8_string.c_str() , utf8_string.length(), &wstr[0] , count )
			, "Failed to convert utf8 string to widestring");
		return wstr;
	}

	std::string WindowsOS::UTF8FromWideString(const std::wstring& wide_string) const
	{
		if (wide_string.empty())
		{
			return std::string();
		}
		size_t count = WideCharToMultiByte( CP_UTF8 , MB_ERR_INVALID_CHARS , wide_string.c_str() , -1, NULL, 0, NULL, NULL);
		MT_CORE_ASSERT(count, "Failed to count ut8_string");
		std::string utf8str;
		utf8str.resize(count);
		MT_CORE_ASSERT(WideCharToMultiByte(CP_UTF8 , MB_ERR_INVALID_CHARS , wide_string.c_str() , wide_string.length(), &utf8str[0] , count, NULL, NULL)
			, "Failed to convert utf8 string to widestring");
		return utf8str;
	}

	// From: https://stackoverflow.com/questions/794632/programmatically-get-the-cache-line-size
	size_t WindowsOS::GetCacheLineSize() const
	{
		size_t lineSize = 0;
		DWORD bufferSize = 0;
		DWORD i = 0;
		SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = nullptr;

		GetLogicalProcessorInformation(0, &bufferSize);
		buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*) malloc(bufferSize);
		GetLogicalProcessorInformation(&buffer[0], &bufferSize);

		for (i = 0; i < bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i)
		{
			if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1)
			{
				lineSize = buffer[i].Cache.LineSize;
				break;
			}
		}

		free(buffer);
		return lineSize;
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
			m_Frequency = 1000;
			m_Offset = (uint64_t)timeGetTime();
		}
	}

	void WindowsOS::LoadLibraries()
	{
		// User32
		m_Libraries.User32.Instance = LoadLibraryA("user32.dll");
		MT_CORE_ASSERT(m_Libraries.User32.Instance, "Failed to load user32");
		m_Libraries.User32.AdjustWindowRectExForDpi = (PFN_AdjustWindowRectExForDpi)
			GetProcAddress(m_Libraries.User32.Instance,"AdjustWindowRectExForDpi");
		m_Libraries.User32.ChangeWindowMessageFilterEx = (PFN_ChangeWindowMessageFilterEx)
			GetProcAddress(m_Libraries.User32.Instance,"ChangeWindowMessageFilterEx");
		m_Libraries.User32.EnableNonClientDpiScaling = (PFN_EnableNonClientDpiScaling)
			GetProcAddress(m_Libraries.User32.Instance,"EnableNonClientDpiScaling");
		m_Libraries.User32.GetDpiForWindow = (PFN_GetDpiForWindow)
			GetProcAddress(m_Libraries.User32.Instance,"GetDpiForWindow");
		m_Libraries.User32.SetProcessDPIAware = (PFN_SetProcessDPIAware)
			GetProcAddress(m_Libraries.User32.Instance,"SetProcessDPIAware");
		m_Libraries.User32.SetProcessDpiAwarnessContext = (PFN_SetProcessDpiAwarenessContext)
			GetProcAddress(m_Libraries.User32.Instance,"SetProcessDpiAwarenessContext");
		
		// Shcore
		m_Libraries.Shcore.Instance = LoadLibraryA("shcore.dll");
		MT_CORE_ASSERT(m_Libraries.Shcore.Instance, "Failed to load shcore");
		m_Libraries.Shcore.GetDpiForMonitor = (PFN_GetDpiForMonitor)
			GetProcAddress(m_Libraries.Shcore.Instance,"GetDpiForMonitor");
		m_Libraries.Shcore.SetProcessDpiAwareness = (PFN_SetProcessDpiAwareness)
			GetProcAddress(m_Libraries.Shcore.Instance,"SetProcessDpiAwareness");
		
		// Ntdll
		m_Libraries.Ntdll.Instance = LoadLibraryA("ntdll.dll");
		MT_CORE_ASSERT(m_Libraries.Ntdll.Instance, "Failed to load ntdll");
		m_Libraries.Ntdll.RtlVerifyVersionInfo = (PFN_RtlVerifyVersionInfo)
			GetProcAddress(m_Libraries.Ntdll.Instance,"RtlVerifyVersionInfo");
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

