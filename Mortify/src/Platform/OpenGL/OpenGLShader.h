#pragma once

#include "Mortify/Rendering/Shader.h"

#include <glm/glm.hpp>

// TODO : REMOVE!
typedef unsigned int GLenum;

namespace Mortify
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filePath);

		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override;

		unsigned int const getProgramID() const { return m_RendererID; }

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMatrix3f(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		GLint GetUniformLocation(const std::string& name) const;

	private:
		std::string m_Name;
		uint32_t m_RendererID;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
	};
}