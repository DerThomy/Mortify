#pragma once

#include "Mortify/Rendering/Texture.h"

#include <glad/glad.h>

namespace Mortify
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind() override;

		virtual uint16_t GetLocation() const override { return m_Location; }

		virtual void SetData(void* data, uint32_t size) override;

	private:
		static std::vector<Texture2D*> slots;

	private:
		uint16_t m_Location;
		uint32_t m_Width, m_Height;
		std::string m_Path;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};
}