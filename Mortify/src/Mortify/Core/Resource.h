#pragma once

#include "mtpch.h"

namespace Mortify
{
	class Resource
	{
	public:
		Resource() : m_SceneID(0), m_Loaded(false) {}
		virtual ~Resource() {}

		inline std::string GetPath() const { return m_Path; }
		inline void SetPath(const std::string& path) { m_Path = path; }

		inline bool IsLoaded() const { return m_Loaded; }
		inline void SetLoaded(bool loaded) { m_Loaded = loaded; }

		virtual void Load() = 0;
		virtual void Unload() = 0;

	protected:
		uint32_t m_SceneID;
		std::string m_Path;
		bool m_Loaded;
	};

	class ResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager() {}
		inline static ResourceManager& GetInstance() { return *s_Instance; }

		template <typename T>
		T* GetResourceByID(std::string resourceID) const;
		const std::list<Resource*>& GetSceneResources(uint32_t sceneID) const;

		void Clear();
		inline void SetCurrentScene(uint32_t sid) { m_CurrentScene = sid; }
		inline const uint32_t GetResourceCount() const { return m_ResourceCount; }

		bool LoadFromXMLFile(std::string filename);

		template <typename T>
		void AddResource(std::string path);

		void LoadResources();
		void UnloadResources();

	protected:
		uint32_t m_CurrentScene;
		uint32_t m_ResourceCount;
		std::unordered_map<uint32_t, std::list<Resource*>> m_Resources;
	private:
		static ResourceManager* s_Instance;
	};
}