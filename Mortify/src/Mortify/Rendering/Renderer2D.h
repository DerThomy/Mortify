#pragma once

#include "Camera.h"

namespace Mortify
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f});
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = { 0.0f, 0.0f, 0.0f, 1.0f});
	};
}