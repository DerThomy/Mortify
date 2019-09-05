#include "mtpch.h"

#include "Resource.h"

namespace Mortify
{
	ResourceManager* ResourceManager::s_Instance = nullptr;

	ResourceManager::ResourceManager()
		: m_CurrentScene(0), m_ResourceCount(0)
	{
		MT_ASSERT(!s_Instance, "Resource Manager already initialized!");
		s_Instance = this;
	}

	template <typename T>
	T* ResourceManager::GetResourceByID(std::string path) const
	{
		auto& resources = m_Resources.find(m_CurrentScene)->second;
		auto resource = std::find_if(resources.begin(), resources.end(), 
			[&](const Resource* r)
			{
				return r->GetPath() == path;
			}
		);

		if (resource == resources.end())
			return nullptr;

		try
		{
			T* result = dynamic_cast<T*>(resource);
		}
		catch (std::bad_cast& b)
		{
			MT_CORE_ERROR("Resource at path {0} is wrong type: {1}", path, b.what())
		}


		return result;
	}

	const std::list<Resource*>& ResourceManager::GetSceneResources(uint32_t sceneID) const
	{
		auto& result = m_Resources.find(sceneID);
		if (result == m_Resources.end())
			return std::list<Resource*>();

		return result->second;
	}

	void ResourceManager::Clear()
	{
		for (auto& sceneResources : m_Resources)
		{
			for (auto* resource : sceneResources.second)
			{
				delete resource;
			}
		}

		m_Resources.clear();
	}

	bool ResourceManager::LoadFromXMLFile(std::string filename)
	{
		return false;
	}

	template <typename T>
	void ResourceManager::AddResource(std::string path)
	{
		MT_CORE_ASSERT(std::is_base_of<Resource, T>::value, "T must be Resource");

		if (m_Resources.find(m_CurrentScene) == m_Resources.end())
			m_Resources.insert({ m_CurrentScene, std::list<Resource*>() });

		auto& resources = m_Resources.find(m_CurrentScene)->second;
		auto& resource = std::find_if(resources.begin(), resources.end(), [&](const Resource * r) { return r->GetPath() == path; });
		if (resource == resources.end())
		{
			Resource* resource = new T();
			resource->SetPath(path);
			m_Resources.at(m_CurrentScene).push_back(resource);
		}
	}

	void ResourceManager::LoadResources()
	{
		for (auto& resource : m_Resources.find(m_CurrentScene)->second)
		{
			if (!resource->IsLoaded())
				resource->Load();
		}
	}

	void ResourceManager::UnloadResources()
	{
		for (auto& resource : m_Resources.find(m_CurrentScene)->second)
		{
			if (resource->IsLoaded())
				resource->Unload();
		}
	}
}