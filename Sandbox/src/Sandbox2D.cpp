#include "Sandbox2D.h"
#include "imgui.h"

#include <imgui.h>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(const Mortify::Ref<Mortify::Window>& window, Mortify::EventCallbackFn AppCallback)
	: Layer("Sandbox2D"), m_CameraController(window->GetAspectRatio())
	, m_Window(window)
	//, m_SecondWindow(Mortify::Window::Create(Mortify::WindowConfig(), AppCallback))
	//, m_CameraController2(m_SecondWindow->GetAspectRatio())
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
	//m_CameraController2.OnUpdate(ts);

	{
		// Render
		MT_PROFILE_SCOPE("Renderer Draw");

		if (m_Window && m_Window->GetContext()->IsValid())
		{
			Mortify::Renderer2D::SetContext(m_Window->GetContext());

			{
				MT_PROFILE_SCOPE("Renderer Prep");
				Mortify::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
				Mortify::RenderCommand::Clear();
			}

			Mortify::Renderer2D::BeginScene(m_CameraController.GetCamera());

			Mortify::Renderer2D::DrawQuad({
				{-1.0f, 0.0f, 0.0f },
				{ 0.8f, 0.8f },
				0.0f,
				{0.8f, 0.2f, 0.3f, 1.0f} }
			);

			Mortify::Renderer2D::DrawQuad({
				{0.5f, -0.5f, 0.0f },
				{ 0.5f, 0.75f },
				1.0f,
				m_SquareColor }
			);

			Mortify::Renderer2D::DrawQuad({
				{ 0.0f, 0.0f, -0.1f },
				{ 10.0f, 10.0f },
				0.0f,
				glm::vec4(1.0f),
				m_CheckerboardTexture,
				10.0f }
			);

			Mortify::Renderer2D::EndScene();
		}

		/*if (m_SecondWindow && m_SecondWindow->GetContext()->IsValid())
		{
			Mortify::Renderer2D::SetContext(m_SecondWindow->GetContext());

			{
				MT_PROFILE_SCOPE("Renderer Prep");
				Mortify::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
				Mortify::RenderCommand::Clear();
			}

			Mortify::Renderer2D::BeginScene(m_CameraController2.GetCamera());

			Mortify::Renderer2D::DrawQuad({
				{-1.0f, 0.0f, 0.0f },
				{ 0.8f, 0.8f },
				0.0f,
				{0.8f, 0.2f, 0.3f, 1.0f} }
			);

			Mortify::Renderer2D::DrawQuad({
				{0.5f, -0.5f, 0.0f },
				{ 0.5f, 0.75f },
				1.0f,
				m_SquareColor }
			);

			Mortify::Renderer2D::DrawQuad({
				{ 0.0f, 0.0f, -0.1f },
				{ 10.0f, 10.0f },
				0.0f,
				glm::vec4(1.0f),
				m_CheckerboardTexture,
				10.0f }
			);

			Mortify::Renderer2D::EndScene();
		}*/
	}
}

std::string Sandbox2D::LimitsToString(const Mortify::WindowLimits& limits)
{
	std::string ret;

	ret += "MinWidth: ";
	ret += limits.MinWidth.has_value() ? std::to_string(limits.MinWidth.value()) : "limitless";
	ret += ", ";

	ret += "MaxWidth: ";
	ret += limits.MaxWidth.has_value() ? std::to_string(limits.MaxWidth.value()) : "limitless";
	ret += " | ";

	ret += "MinHeight: ";
	ret += limits.MinHeight.has_value() ? std::to_string(limits.MinHeight.value()) : "limitless";
	ret += ", ";

	ret += "MaxHeight: ";
	ret += limits.MaxHeight.has_value() ? std::to_string(limits.MaxHeight.value()) : "limitless";

	return ret;
}

void Sandbox2D::OnImGuiRender()
{
	MT_PROFILE_FUNCTION();
	
	//ImGui::Begin("Demo");

	//ImGui::ShowDemoWindow();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::Text(("Width: " + std::to_string(m_Window->GetWidth())).c_str());
	ImGui::Text(("Height :" + std::to_string(m_Window->GetHeight())).c_str());
	ImGui::Text(("Mode: " + Mortify::windowModeToString.at(m_Window->GetWindowMode())).c_str());
	ImGui::Text(("Limits: " + LimitsToString(m_Window->GetLimits())).c_str());
	ImGui::Text(("Resizeable: " + std::string(m_Window->IsResizeable() ? "true" : "false")).c_str());
	ImGui::Text(("KeepAspect: " + std::string(m_Window->KeepsAspectRatio() ? "true" : "false")).c_str());
	ImGui::Text(("VSync: " + std::string(m_Window->VSyncEnabled() ? "true" : "false")).c_str());
	ImGui::Text(("Maximized: " + std::string(m_Window->IsMaximized() ? "true" : "false")).c_str());
	ImGui::Text(("Minimized: " + std::string(m_Window->IsMinimized() ? "true" : "false")).c_str());
	ImGui::Text(("Fullscreen: " + std::string(m_Window->IsFullscreen() ? "true" : "false")).c_str());
	ImGui::Text(("Borderless: " + std::string(m_Window->IsBorderless() ? "true" : "false")).c_str());
	
	ImGui::End();
}

void Sandbox2D::OnEvent(Mortify::Event& e)
{
	m_CameraController.OnEvent(e);
	//m_CameraController2.OnEvent(e);
}