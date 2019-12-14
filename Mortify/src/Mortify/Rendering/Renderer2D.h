#pragma once

#include "Mortify/Rendering/Camera.h"
#include "Mortify/Rendering/Texture.h"

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
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
	};
}