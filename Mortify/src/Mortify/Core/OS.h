#pragma once

namespace Mortify
{
	class OS
	{
	public:
		static Ref<OS> Create();

		virtual double GetTime() const = 0;

		static Ref<OS>& GetOS() { return s_Instance; }
	private:
		static Ref<OS>& s_Instance;
	};
}