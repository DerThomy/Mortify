#include "mtpch.h"

#include "Mesh.h"

namespace Mortify
{
	Mesh::Mesh(const Ref<Material>& material)
		: m_Material(material)
	{
		
	}

	const Ref<Mesh>& Mesh::GetSubMeshByID(std::string path) const
	{
		for (auto& mesh : m_SubMeshes)
		{
			if (mesh->GetPath() == path)
				return mesh;

			return mesh->GetSubMeshByID(path);
		}
		return nullptr;
	}

	void Mesh::SetTransform(const glm::mat4& transform)
	{
		m_TransformMatrix = transform;

		for (auto& mesh : m_SubMeshes)
		{
			mesh->SetTransform(transform);
		}
	}
}