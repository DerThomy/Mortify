#pragma once

#include "Mortify/Core/OS.h"

namespace Mortify
{
	class WindowsOS : public OS
	{
	public:
		WindowsOS();

		virtual double GetTime() const override;
	private:
		uint64_t	m_Offset;
		uint64_t	m_Frequency;
		bool		m_hasPC;
	};
}