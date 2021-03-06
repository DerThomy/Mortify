#include "mtpch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Mortify/Rendering/RenderContext.h"

#include <glad/glad.h>

namespace Mortify
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         MT_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       MT_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          MT_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: MT_CORE_TRACE(message); return;
		}

		MT_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init(const Ref<RenderContext>& context)
	{
		MT_PROFILE_FUNCTION();

		context->MakeContextCurrent();

	#if MT_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}