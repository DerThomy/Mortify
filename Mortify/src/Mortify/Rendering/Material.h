#pragma once

#include "Shader.h"
#include "Mortify/Core/Resource.h"
#include "Mortify/Rendering/Texture.h"

namespace Mortify
{
	class Material : Resource
	{
	public:
		Material(const Ref<Shader>& shader)
			: m_Shader(shader)
		{}

		inline void SetShader(const Ref<Shader>& shader) { m_Shader = shader; }
		inline const Ref<Shader>& GetShader() const { return m_Shader; }

	private:
		Ref<Shader> m_Shader;

		std::vector<Texture> m_Textures;
	};
}