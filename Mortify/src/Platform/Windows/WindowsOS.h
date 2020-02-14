#pragma once

#include "Mortify/Core/OS.h"
#include <Windows.h>

	#ifndef DPI_ENUMS_DECLARED
	typedef enum
	{
	    PROCESS_DPI_UNAWARE = 0,
	    PROCESS_SYSTEM_DPI_AWARE = 1,
	    PROCESS_PER_MONITOR_DPI_AWARE = 2
	} PROCESS_DPI_AWARENESS;
	typedef enum
	{
	    MDT_EFFECTIVE_DPI = 0,
	    MDT_ANGULAR_DPI = 1,
	    MDT_RAW_DPI = 2,
	    MDT_DEFAULT = MDT_EFFECTIVE_DPI
	} MONITOR_DPI_TYPE;
	#endif

	// Typedefs Ntdll.dll
	typedef LONG (WINAPI * PFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*,ULONG,ULONGLONG);

	// Typedefs User32.dll
	typedef BOOL (WINAPI * PFN_SetProcessDPIAware)(void);
	typedef BOOL (WINAPI * PFN_ChangeWindowMessageFilterEx)(HWND,UINT,DWORD,CHANGEFILTERSTRUCT*);
	typedef BOOL (WINAPI * PFN_EnableNonClientDpiScaling)(HWND);
	typedef BOOL (WINAPI * PFN_SetProcessDpiAwarenessContext)(HANDLE);
	typedef UINT (WINAPI * PFN_GetDpiForWindow)(HWND);
	typedef BOOL (WINAPI * PFN_AdjustWindowRectExForDpi)(LPRECT,DWORD,BOOL,DWORD,UINT);

	// Typedefs Shcore.dll
	typedef HRESULT (WINAPI * PFN_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);
	typedef HRESULT (WINAPI * PFN_GetDpiForMonitor)(HMONITOR,MONITOR_DPI_TYPE,UINT*,UINT*);

namespace Mortify
{
	struct USER32
	{
		HMODULE Instance;
		
		PFN_SetProcessDPIAware				SetProcessDPIAware;
		PFN_ChangeWindowMessageFilterEx		ChangeWindowMessageFilterEx;
		PFN_EnableNonClientDpiScaling		EnableNonClientDpiScaling;
		PFN_SetProcessDpiAwarenessContext	SetProcessDpiAwarnessContext;
		PFN_GetDpiForWindow					GetDpiForWindow;
		PFN_AdjustWindowRectExForDpi		AdjustWindowRectExForDpi;
	};

	struct SHCORE
	{
		HMODULE Instance;
		
		PFN_SetProcessDpiAwareness	SetProcessDpiAwareness;
		PFN_GetDpiForMonitor		GetDpiForMonitor;
	};
	
	struct NTDLL
	{
		HMODULE Instance;
		
		PFN_RtlVerifyVersionInfo RtlVerifyVersionInfo;
	};
	
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