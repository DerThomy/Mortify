#pragma once

#include "Mortify.h"

class Sandbox2D : public Mortify::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Mortify::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Mortify::Event& e) override;
private:
	Mortify::OrthoCameraController m_CameraController;

	// Temp
	Mortify::Ref<Mortify::VertexArray> m_SquareVA;
	Mortify::Ref<Mortify::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};