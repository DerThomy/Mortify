#include "mtpch.h"

#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Mortify
{
	OpenGLShader::OpenGLShader(const std::string& filePath)
		: m_RendererID(0), m_AssetPath(filePath)
	{
		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);

		Reload();
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_RendererID(0), m_Name(name), m_AssetPath(std::string())
	{
		m_ShaderSources[GL_VERTEX_SHADER] = vertexSrc;
		m_ShaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile();

		m_Loaded = true;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Reload()
	{
		if (m_AssetPath == std::string())
		{
			MT_CORE_ERROR("Cant reload shader '{0}', scince its source is provided by strings", m_Name);
			return;
		}

		std::string sourceCode = ReadFile(m_AssetPath);
		m_ShaderSources = PreProcess(sourceCode);
		Parse();

		if (m_RendererID)
			glDeleteShader(m_RendererID);

		for (auto& callback : m_ShaderReloadedCallbacks)
			callback();

		Compile();

		m_Loaded = true;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	std::optional<ShaderStruct*> OpenGLShader::FindStruct(const std::string& name)
	{
		for (ShaderStruct* s : m_Structs)
		{
			if (s->GetName() == name)
				return s;
		}

		return {};
	}

	GLenum OpenGLShader::ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		MT_CORE_ASSERT(false, "Shader type not supported!");
		return 0;
	}

	void OpenGLShader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
	{
		m_ShaderReloadedCallbacks.push_back(callback);
	}

	void OpenGLShader::UploadUniformBuffer(const UniformBufferBase& uniformBuffer)
	{
		for (unsigned int i = 0; i < uniformBuffer.GetUniformCount(); i++)
		{
			const UniformDecl& decl = uniformBuffer.GetUniforms()[i];
			const std::string& name = decl.Name;

			switch (decl.Type)
			{
				case UniformType::Float:
				{
					float value = *(float*)(uniformBuffer.GetBuffer() + decl.Offset);
					UploadUniformFloat(name, value);
					break;
				}
				case UniformType::Float2:
				{
					glm::vec2 value = *(glm::vec2*)(uniformBuffer.GetBuffer() + decl.Offset);
					UploadUniformFloat2(name, value);
					break;
				}
				case UniformType::Float3:
				{
					glm::vec3& value = *(glm::vec3*)(uniformBuffer.GetBuffer() + decl.Offset);
					UploadUniformFloat3(name, value);
					break;
				}
				case UniformType::Float4:
				{
					glm::vec4& value = *(glm::vec4*)(uniformBuffer.GetBuffer() + decl.Offset);
					UploadUniformFloat4(name, value);
					break;
				}
				case UniformType::Mat3x3:
				{
					glm::mat3 value = *(glm::mat3*)(uniformBuffer.GetBuffer() + decl.Offset);
					UploadUniformMatrix3f(name, value);
					break;
				}
				case UniformType::Mat4x4:
				{
					glm::mat4 value = *(glm::mat4*)(uniformBuffer.GetBuffer() + decl.Offset);
					UploadUniformMatrix4f(name, value);
					break;
				}
				default:
				{
					MT_CORE_ERROR("Uniform type not yet supported! ({1})", decl.Name);
				}
			}
		}
	}

	void OpenGLShader::SetVSMaterialUniformBuffer(Buffer buffer)
	{

	}

	void OpenGLShader::SetPSMaterialUniformBuffer(Buffer buffer)
	{

	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			MT_CORE_WARN("Could not open '{0}'", filePath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			MT_CORE_ASSERT(eol != std::string::npos, "Syntax error!");
			std::string line = source.substr(pos, eol - pos);
			size_t begin = source.find_first_not_of(" ", pos + typeTokenLength);
			MT_CORE_ASSERT(begin != eol, "Missing Type!");
			std::string type = source.substr(begin, eol - begin);
			MT_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Parse()
	{
	}

	void OpenGLShader::ParseUniform(const std::string& statement, ShaderDomain domain)
	{
	}

	void OpenGLShader::ParseUniformStruct(const std::string& block, ShaderDomain domain)
	{
	}

	void OpenGLShader::ResolveUniforms()
	{

	}

	void OpenGLShader::ValidateUniforms()
	{
	}

	void OpenGLShader::Compile()
	{
		GLuint program = glCreateProgram();

		MT_CORE_ASSERT(m_ShaderSources.size() <= 2, "More than 2 shaders are currently not supported");
		std::array<GLenum, 2> shaderIDs;

		int glShaderIDIndex = 0;

		for (auto& kv : m_ShaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				MT_CORE_ERROR("{0}", infoLog.data());
				MT_CORE_ASSERT(false, "Shader compilation failed!");
				return;
			}

			glAttachShader(program, shader);
			shaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)& isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			for (auto id : shaderIDs)
				glDeleteShader(id);

			MT_CORE_ERROR("\n{0}", infoLog.data());
			MT_CORE_ASSERT(false, "Shader link failed!");
			return;
		}
		
		for (auto id : shaderIDs)
			glDetachShader(m_RendererID, id);

		m_RendererID = program;
	}

	void OpenGLShader::ResolveAndSetUniforms(const Scope<OpenGLShaderUniformBufferDeclaration>& decl, Buffer buffer)
	{
	}

	void OpenGLShader::ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, Buffer buffer)
	{
	}

	void OpenGLShader::ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, Buffer buffer)
	{
	}

	void OpenGLShader::ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, byte* data, int32_t offset)
	{
	}

	GLint OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_RendererID, name.c_str());

		if (location == -1)
		{
			MT_CORE_WARN("Uniform '{0}' could not be found!", name);
			return location;
		}

		m_UniformLocationCache[name] = location;
		return location;
	}

	void OpenGLShader::UploadUniformInt(uint32_t location, int32_t value)
	{
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(uint32_t location, int32_t* values, int32_t count)
	{
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(uint32_t location, float value)
	{
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(uint32_t location, const glm::vec2& value)
	{
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(uint32_t location, const glm::vec3& value)
	{
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(uint32_t location, const glm::vec4& value)
	{
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMatrix3f(uint32_t location, const glm::mat3& values)
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(values));
	}

	void OpenGLShader::UploadUniformMatrix4f(uint32_t location, const glm::mat4& values)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(values));
	}

	void OpenGLShader::UploadUniformMatrix4fArray(uint32_t location, const glm::mat4& values, uint32_t count)
	{
		glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(values));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int32_t value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformInTArray(const std::string& name, int32_t* values, int32_t count)
	{
		GLint location = GetUniformLocation(name);
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMatrix3f(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMatrix4f(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}