#pragma once

#include "Mortify.h"

class Sandbox2D : public Mortify::Layer
{
public:
	Sandbox2D(Mortify::Window& window);
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Mortify::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Mortify::Event& e) override;
private:
	Mortify::OrthoCameraController m_CameraController;

	Mortify::Ref<Mortify::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};