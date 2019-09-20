#pragma once

#include "Mortify/Core/Log.h"

namespace Mortify
{
	class Resource
	{
	public:
		Resource() : m_Loaded(false) {}
		virtual ~Resource() {}

		inline std::string GetPath() const { return m_Path; }

		inline void SetName(const std::string& name) { m_Name = name; }
		inline const std::string& GetName() const { return m_Name; }

	protected:
		inline const std::string& GetNameFromPath(const std::string& filePath)
		{
			auto lastSlash = filePath.find_last_of("/\\");
			lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
			auto lastDot = filePath.rfind('.');
			auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
			m_Name = filePath.substr(lastSlash, count);
		}

	protected:
		std::string m_Name;
		std::string m_Path;
		bool m_Loaded;
	};

	class ResourceManager
	{
	public:
		void Add(const Ref<Resource>& res)
		{
			MT_CORE_ASSERT(!Exists(res->GetName()), "Resource already exists");
			m_Resources[res->GetName()] = res;
		}

		void Add(const std::string& name, const Ref<Resource>& res)
		{
			MT_CORE_ASSERT(!Exists(name), "Resource already exists");
			m_Resources[name] = res;
		}

		virtual Ref<Resource> Load(const std::string& filePath) = 0;

		Ref<Resource> Get(const std::string& name)
		{
			MT_CORE_ASSERT(Exists(name), "Resource doesnt exist!");
			return m_Resources[name];
		}

		bool Exists(const std::string& name)
		{
			return m_Resources.find(name) != m_Resources.end();
		}

	private:
		std::unordered_map<std::string, Ref<Resource>> m_Resources;
	};
}