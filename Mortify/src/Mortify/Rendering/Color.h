#pragma once

namespace Mortify
{
	struct Color
	{
	private:
		union
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
			float components[4];
		};
	public:
		Color()
		{
			Set(0, 0, 0, 1);
		}

		Color(float InR, float InG, float InB, float InA = 1)
		{
			Set(InR, InG, InB, InA);
		}

		void Set(float InR, float InG, float InB, float InA)
		{
			r = InR;
			g = InG;
			b = InB;
			a = InA;
		}

		float GetRed() const { return r; }
		float GetGreen() const { return g; }
		float GetBlue() const { return b; }
		float GetAlpha() const { return a; }

		bool operator== (const Color& InColor) { return (r == InColor.r && g == InColor.g && b == InColor.b && a == InColor.a); }
		bool operator!= (const Color& InColor) { return (r != InColor.r || g != InColor.g || b != InColor.b || a != InColor.a); }

		float& operator[] (int idx)
		{
			return components[idx];
		}

		const float& operator[] (int idx) const
		{
			return components[idx];
		}
	};
}