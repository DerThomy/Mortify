#pragma once

#include <Mortify.h>

class ExampleLayer : public Mortify::Layer
{
public:
	ExampleLayer(const Mortify::Ref<Mortify::Window>& window);

	void OnUpdate(Mortify::Timestep ts) override;

	virtual void OnImGuiRender() override;

	void OnEvent(Mortify::Event& event) override;

private:
	
	Mortify::Ref<Mortify::Window> m_Window;

	Mortify::ShaderLibrary m_ShaderLibrary;
	Mortify::Ref<Mortify::Shader> m_Shader;
	Mortify::Ref<Mortify::VertexArray> m_VertexArray;

	Mortify::Ref<Mortify::Shader> m_FlatColorShader;
	Mortify::Ref<Mortify::VertexArray> m_SquareVA;

	Mortify::Ref<Mortify::Texture2D> m_Texture, m_ChernoLogoTexture;

	Mortify::OrthoCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};