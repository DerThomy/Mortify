#pragma once

#include "VertexArray.h"
#include "Material.h"
#include "Mortify/Core/Resource.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Mortify
{
	class Mesh : Resource
	{
	public:
		struct Surface
		{

		};

		Mesh(const Ref<Material>& material);

		virtual ~Mesh() {};
		
		inline const Ref<Material>& GetMaterial() const { return m_Material; }
		inline void SetMaterial(const Ref<Material>& material) { m_Material = material; }

		inline const glm::mat4 GetTransformMatrix() const { return m_TransformMatrix; }
		void SetTransform(const glm::mat4& transform);

		inline const std::vector<Ref<Mesh>>& GetSubMeshes() const { return m_SubMeshes; }
		inline void SetSubMeshes(const std::vector<Ref<Mesh>>& subMeshes) { m_SubMeshes = subMeshes; }
		inline void AddSubMesh(const Ref<Mesh>& mesh) { m_SubMeshes.push_back(mesh); }
		inline void ClearSubMeshes() { m_SubMeshes.clear(); }
	
		const Ref<Mesh>& GetSubMeshByID(std::string path) const;

		inline const float* const GetVertices() const { return m_Vertices; }

		inline const uint32_t* const GetIndices() const { return m_Indices; }

	private:
		float* m_Vertices;
		uint32_t* m_Indices;
		glm::mat4 transform = glm::mat4(1.0f);
		Ref<Material> m_Material;
		glm::mat4 m_TransformMatrix;

		std::vector<Ref<Mesh>> m_SubMeshes;
	};
}