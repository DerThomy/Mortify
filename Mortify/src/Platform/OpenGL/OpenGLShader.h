#pragma once

#include "Mortify/Rendering/Shader.h"
#include "OpenGLShaderUniform.h"
#include "OpenGLRendererAPI.h"

#include <glm/glm.hpp>

// TODO : REMOVE!
typedef unsigned int GLenum;
typedef int GLint;

namespace Mortify
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& filePath);

		virtual ~OpenGLShader();

		virtual void Reload() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;

		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) override;

		inline RendererID const GetProgramID() const { return m_RendererID; }
		virtual const std::string& GetName() const override { return m_Name; }

		virtual void SetVSMaterialUniformBuffer(Buffer buffer) override;
		virtual void SetPSMaterialUniformBuffer(Buffer buffer) override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

	public:
		static GLenum ShaderTypeFromString(const std::string& type);

		void UploadUniformInt(uint32_t location, int32_t value);
		void UploadUniformIntArray(uint32_t location, int32_t* values, int32_t count);
		void UploadUniformFloat(uint32_t location, float value);
		void UploadUniformFloat2(uint32_t location, const glm::vec2& value);
		void UploadUniformFloat3(uint32_t location, const glm::vec3& value);
		void UploadUniformFloat4(uint32_t location, const glm::vec4& value);
		void UploadUniformMatrix3f(uint32_t location, const glm::mat3& values);
		void UploadUniformMatrix4f(uint32_t location, const glm::mat4& values);
		void UploadUniformMatrix4fArray(uint32_t location, const glm::mat4& values, uint32_t count);

		void UploadUniformStruct(OpenGLShaderUniformDeclaration* uniform, byte* buffer, uint32_t offset);

		void UploadUniformInt(const std::string& name, int32_t value);
		void UploadUniformIntArray(const std::string& name, int32_t* values, int32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMatrix3f(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMatrix4f(const std::string& name, const glm::mat4& matrix);

		inline virtual const ShaderUniformBufferList& GetVSRendererUniforms() const override { return m_VSRendererUniformBuffers; }
		inline virtual const ShaderUniformBufferList& GetPSRendererUniforms() const override { return m_PSRendererUniformBuffers; }
		inline virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const override { return *m_VSMaterialUniformBuffer; }
		inline virtual const ShaderUniformBufferDeclaration& GetPSMaterialUniformBuffer() const override { return *m_PSMaterialUniformBuffer; }
		inline virtual const ShaderResourceList& GetResources() const override { return m_Resources; }

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Parse();
		void ParseUniform(const std::string& statement, ShaderDomain domain);
		void ParseUniformStruct(const std::string& block, ShaderDomain domain);

		std::optional<ShaderStruct*> FindStruct(const std::string& name);

		GLint GetUniformLocation(const std::string& name) const;

		void ResolveShaderUniformList(const ShaderUniformList& uniforms);

		void ResolveUniforms();
		void ValidateUniforms();
		void Compile();

		void ResolveAndSetUniforms(const Scope<OpenGLShaderUniformBufferDeclaration>& decl, Buffer buffer);
		void ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, Buffer buffer);
		void ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, Buffer buffer);
		void ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, byte* data, int32_t offset);

	private:
		std::string m_Name, m_AssetPath;
		RendererID m_RendererID;
		bool m_Loaded = false;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
		std::unordered_map<GLenum, std::string> m_ShaderSources;

		std::vector<ShaderReloadedCallback> m_ShaderReloadedCallbacks;

		ShaderUniformBufferList m_VSRendererUniformBuffers;
		ShaderUniformBufferList m_PSRendererUniformBuffers;
		Scope<OpenGLShaderUniformBufferDeclaration> m_VSMaterialUniformBuffer;
		Scope<OpenGLShaderUniformBufferDeclaration> m_PSMaterialUniformBuffer;
		ShaderResourceList m_Resources;
		ShaderStructList m_Structs;
	};
}