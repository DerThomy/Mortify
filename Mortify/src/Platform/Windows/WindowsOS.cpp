#include "mtpch.h"

#include "WindowsOS.h"

#include <Windows.h>
#include <timeapi.h>

namespace Mortify
{
	WindowsOS::WindowsOS()
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

	double WindowsOS::GetTime() const
	{
		uint64_t value;

		if (m_hasPC)
			QueryPerformanceCounter((LARGE_INTEGER*) &value);
		else
			value = (uint64_t)timeGetTime();

		return (double)(value - m_Offset) / m_Frequency;
	}
}

