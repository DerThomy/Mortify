#pragma once

#include "Mortify/Core/OS.h"
#include "Platform/Windows/WinodowsCore.h"

#include <ShObjIdl.h>
#include <wrl/client.h>

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
		const Microsoft::WRL::ComPtr<ITaskbarList2>& GetTaskbarList() const { return m_TaskbarList; }
		const Libraries& GetLibraries() const { return m_Libraries; };
		float GetDpiForMonitor(HMONITOR monitor);

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
		void InitTaskbarList();
		
		BOOL IsWindowsVersionOrGreater(WORD major, WORD minor, WORD sp);
		BOOL IsWindows10BuildOrGreater(WORD build);
		
	private:		
		uint64_t	m_Offset;
		uint64_t	m_Frequency;
		bool		m_hasPC;

		Libraries m_Libraries;

		Microsoft::WRL::ComPtr<ITaskbarList2> m_TaskbarList;
	};
}