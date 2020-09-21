#include "mtpch.h"

#include "Mortify/Rendering/Shader.h"
#include "Mortify/Rendering/Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Mortify
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::Nil: MT_CORE_ASSERT(false, "RenderAPI::Nil is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::Nil: MT_CORE_ASSERT(false, "RenderAPI::Nil is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(filePath);
		}

		MT_CORE_ASSERT(false, "RenderAPI is not supported");
		return nullptr;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		MT_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto name = shader->GetName();
		MT_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		MT_CORE_ASSERT(Exists(name), "Shader doesnt exist!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
    
    template<> template<> void UniformBufferDeclaration<U, N>::Push(const std::string& name, const float& data)
    {
        Uniforms[Index++] = { UniformType::Float, Cursor, name };
        memcpy(Buffer + Cursor, &data, sizeof(float));
        Cursor += sizeof(float);
    }
    
    template<> template<> void UniformBufferDeclaration<U, N>::Push(const std::string& name, const glm::vec3& data)
    {
        Uniforms[Index++] = { UniformType::Float3, Cursor, name };
        memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec3));
        Cursor += sizeof(glm::vec3);
    }

    template<> template<> void UniformBufferDeclaration<U, N>::Push(const std::string& name, const glm::vec4& data)
    {
        Uniforms[Index++] = { UniformType::Float4, Cursor, name };
        memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec4));
        Cursor += sizeof(glm::vec4);
    }

    template<> template<> void UniformBufferDeclaration<U, N>::Push(const std::string& name, const glm::mat4& data)
    {
        Uniforms[Index++] = { UniformType::Mat4x4, Cursor, name };
        memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::mat4));
        Cursor += sizeof(glm::mat4);
    }
}