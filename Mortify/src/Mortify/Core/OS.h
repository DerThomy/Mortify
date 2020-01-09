#pragma once

namespace Mortify
{
	class OS
	{
	public:
		virtual ~OS() = default;
		
		static Ref<OS> Create();
		
		virtual double GetTime() const = 0;
		virtual std::wstring WideCharFromUTF8(const std::string& utf8_string) const = 0;

		static Ref<OS>& GetOS() { return s_Instance; }
	private:
		static Ref<OS>& s_Instance;
	};
}