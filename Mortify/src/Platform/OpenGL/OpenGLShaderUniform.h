#pragma once

#include "Mortify/Rendering/ShaderUniform.h"

namespace Mortify
{
	class OpenGLShaderResourceDeclaration : public ShaderResourceDeclaration
	{
	public:
		enum class Type
		{
			NONE, TEXTURE2D, TEXTURECUBE
		};

	private:
		friend class OpenGLShader;

	public:
		OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32_t count);

		inline virtual const std::string& GetName() const override { return m_Name; }
		inline virtual uint32_t GetRegister() const override { return m_Register; }
		inline virtual uint32_t GetCount() const override { return m_Count; }

		inline Type GetType() const { return m_Type; }

	public:
		static Type StringToType(const std::string& type);
		static std::string TypeToString(Type type);

	private:
		std::string m_Name;
		uint32_t m_Register = 0;
		uint32_t m_Count;
		Type m_Type;
	};

	class OpenGLShaderUniformDeclaration : public ShaderUniformDeclaration
	{
	public:
		enum class Type
		{
			NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32, STRUCT
		};

	private:
		friend class OpenGLShader;
		friend class OpenGLShaderUniformBufferDeclaration;

	public:
		OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count = 1);
		OpenGLShaderUniformDeclaration(ShaderDomain domain, ShaderStruct* uniformStruct, const std::string& name, uint32_t count = 1);
		
		inline virtual const std::string& GetName() const override { return m_Name; }
		inline virtual uint32_t GetSize() const override { return m_Size; }
		inline virtual uint32_t GetCount() const override { return m_Count; }
		inline virtual uint32_t GetOffset() const override { return m_Offset; }
		inline virtual ShaderDomain GetDomain() const override { return m_Domain; }

		inline uint32_t GetAbsoluteOffset() const { return m_Struct ? m_Struct->GetOffset() + m_Offset : m_Offset; }
		inline int32_t GetLocation() const { return m_Location; }
		inline Type GetType() const { return m_Type; }
		inline bool IsArray() const { return m_Count > 1; }
		inline const ShaderStruct& GetShaderUniformStruct() const { MT_CORE_ASSERT(m_Struct, "Uniform is not part of a struct"); return *m_Struct; }

	public:
		static uint32_t SizeOfUniformType(Type type);
		static Type StringToType(const std::string& type);
		static std::string TypeToString(Type type);

	protected:
		virtual void SetOffset(uint32_t offset) override;

	private:
		std::string m_Name;
		uint32_t m_Size;
		uint32_t m_Count;
		uint32_t m_Offset;
		ShaderDomain m_Domain;

		Type m_Type;
		ShaderStruct* m_Struct;
		mutable int32_t m_Location;
	};

	struct OpenGLShaderUniformField
	{
		OpenGLShaderUniformDeclaration::Type Type;
		std::string Name;
		uint32_t Count;
		mutable uint32_t Size;
		mutable int32_t Location;
	};

	class OpenGLShaderUniformBufferDeclaration : public ShaderUniformBufferDeclaration
	{
	private:
		friend class Shader;

	public:
		OpenGLShaderUniformBufferDeclaration(const std::string& name, ShaderDomain domain);

		void PushUniform(OpenGLShaderUniformDeclaration* uniform);

		inline virtual const std::string& GetName() const override { return m_Name; }
		inline virtual uint32_t GetRegister() const override { return m_Register; }
		inline virtual uint32_t GetSize() const override { return m_Size; }
		inline virtual const ShaderUniformList& GetUniformDeclarations() const override { return m_Uniforms; }

		inline ShaderDomain GetDomain() const { return m_Domain; }

		virtual ShaderUniformDeclaration* FindUniform(const std::string& name) override;

	private:
		std::string m_Name;
		uint32_t m_Register;
		uint32_t m_Size;
		ShaderDomain m_Domain;
		ShaderUniformList m_Uniforms;
	};
}