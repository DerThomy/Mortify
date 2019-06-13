#pragma once

#include <string>

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
	private:
		uint32_t m_RendererID;
	};
}