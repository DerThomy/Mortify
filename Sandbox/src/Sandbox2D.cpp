#include "Sandbox2D.h"
#include "imgui.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(Mortify::Window& window)
	: Layer("Sandbox2D"), m_CameraController(window.GetAspectRatio())
{
}

void Sandbox2D::OnAttach()
{
	MT_PROFILE_FUNCTION();
	
	m_CheckerboardTexture = Mortify::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	MT_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Mortify::Timestep ts)
{
	MT_PROFILE_FUNCTION();
	// Update

	m_CameraController.OnUpdate(ts);

	// Render
	{
		MT_PROFILE_SCOPE("Renderer Prep");
		Mortify::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Mortify::RenderCommand::Clear();
	}

	{
		MT_PROFILE_SCOPE("Renderer Draw");
		Mortify::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		Mortify::Renderer2D::DrawQuad({
			{-1.0f, 0.0f, 0.0f },
			{ 0.8f, 0.8f },
			0.0f,
			{0.8f, 0.2f, 0.3f, 1.0f}}
		);
		
		Mortify::Renderer2D::DrawQuad({
			{0.5f, -0.5f, 0.0f },
			{ 0.5f, 0.75f },
			1.0f,
			{0.2f, 0.3f, 0.8f, 1.0f}}
		);
		
		Mortify::Renderer2D::DrawQuad({
			{ 0.0f, 0.0f, -0.1f },
			{ 10.0f, 10.0f },
			0.0f,
			glm::vec4(1.0f),
			m_CheckerboardTexture,
			10.0f}
		);
		
		Mortify::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	MT_PROFILE_FUNCTION();
	
	ImGui::Begin("Demo");

	//ImGui::ShowDemoWindow();

	//ImGui::Begin("Settings");
	//ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	
	ImGui::End();
}

void Sandbox2D::OnEvent(Mortify::Event& e)
{
	m_CameraController.OnEvent(e);
}