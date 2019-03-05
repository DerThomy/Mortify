#pragma once

namespace Mortify
{
	struct Color
	{
	public:
		Color()
		{
			Set(0, 0, 0, 0);
		}

		Color(float InR, float InG, float InB, float InA = 1)
		{
			Set(InR, InG, InB, InA);
		}

		void Set(float InR, float InG, float InB, float InA)
		{
			m_Data[0] = InR;
			m_Data[1] = InG;
			m_Data[2] = InB;
			m_Data[3] = InA;
		}

		float GetRed() const { return m_Data[0]; }
		float GetGreen() const { return m_Data[1]; }
		float GetBlue() const { return m_Data[2]; }
		float GetAlpha() const { return m_Data[3]; }
	private:
		float m_Data[4];
	};
}