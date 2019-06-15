#pragma once

namespace Mortify
{
	enum class RenderAPI
	{
		None = 0, OpenGL
	};

	class Renderer
	{
	public:
		static inline RenderAPI GetAPI() { return m_Api; }
	private:
		static RenderAPI m_Api;
	};
}