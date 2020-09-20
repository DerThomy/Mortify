#pragma once

#include "Mortify/Core/OS.h"

#include <sys/time.h>

namespace Mortify
{	
	class LinuxOS : public OS
	{
	public:
		LinuxOS();
		~LinuxOS();

		void Init();
		
		double GetTime() const override;
		std::wstring WideCharFromUTF8(const std::string& utf8_string) const override;
		std::string UTF8FromWideString(const std::wstring& wide_string) const override;

		size_t GetCacheLineSize() const override;
        
    private:
		void InitTimer();
        
	private:		
		timespec m_Offset;
	};
}