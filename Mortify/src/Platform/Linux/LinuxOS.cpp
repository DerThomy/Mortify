#include "mtpch.h"

#include "LinuxOS.h"

#include <clocale>
#include <cstdlib>
#include <unistd.h>
#include <wchar.h>
#include <stdio.h>

#define BILLION 1E9

namespace Mortify
{
	LinuxOS::LinuxOS()
	{
		LinuxOS::Init();
	}

	LinuxOS::~LinuxOS()
	{

	}

	Ref<OS> OS::Create() 
	{
		return CreateRef<LinuxOS>();
	}

	void LinuxOS::Init()
	{
		InitTimer();
	}

	double LinuxOS::GetTime() const
	{
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (ts.tv_sec - m_Offset.tv_sec) + (ts.tv_nsec - m_Offset.tv_nsec) / BILLION;
	}

	std::wstring LinuxOS::WideCharFromUTF8(const std::string& utf8_string) const
	{
		std::string curLocale = setlocale(LC_ALL, ""); 
        const char* _Source = utf8_string.c_str();
        size_t _Dsize = std::mbstowcs(NULL, _Source, 0) + 1;
        wchar_t* _Dest = new wchar_t[_Dsize];
        wmemset(_Dest, 0, _Dsize);
        std::mbstowcs(_Dest,_Source,_Dsize);
        std::wstring result = _Dest;
        delete []_Dest;
        setlocale(LC_ALL, curLocale.c_str());
        return result;
	}

	std::string LinuxOS::UTF8FromWideString(const std::wstring& wide_string) const
	{
		std::string curLocale = setlocale(LC_ALL, ""); 
        const wchar_t* _Source = wide_string.c_str();
        size_t _Dsize = std::wcstombs(NULL, _Source, 0) + 1;
        char* _Dest = new char[_Dsize];
        memset(_Dest, 0, _Dsize);
        std::wcstombs(_Dest,_Source,_Dsize);
        std::string result = _Dest;
        delete []_Dest;
        setlocale(LC_ALL, curLocale.c_str());
        return result;
	}

	size_t LinuxOS::GetCacheLineSize() const
	{
		return sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
	}

	void LinuxOS::InitTimer()
	{
        clock_gettime(CLOCK_MONOTONIC, &m_Offset);
	}

}

