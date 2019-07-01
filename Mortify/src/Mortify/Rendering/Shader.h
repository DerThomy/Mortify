#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Mortify
{
	class Shader
	{
	public:
		Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		Shader::~Shader();

		unsigned int const getProgramID() const { return m_RendererID; }

		void Bind() const;
		void Unbind() const;

		void UploadUniformMatrix4f(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
	};
}