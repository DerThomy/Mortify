#pragma once

namespace Mortify
{
	class Timestep
	{
	public:
		Timestep(double time = 0.0f)
			: m_Time(time)
		{}

		operator double() { return m_Time; }

		inline double GetSeconds() const { return m_Time; }
		inline double GetMilliseconds() const { return m_Time * 1000.0f; }
	private:
		double m_Time;
	};
}