#pragma once

#include "Mortify.h"

class Sandbox2D : public Mortify::Layer
{
public:
	Sandbox2D(const Mortify::Ref<Mortify::Window>& window, Mortify::EventCallbackFn AppCallback);
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Mortify::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Mortify::Event& e) override;
private:
	std::string LimitsToString(const Mortify::WindowLimits& limits);

	Mortify::OrthoCameraController m_CameraController;

	Mortify::Ref<Mortify::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	Mortify::Timestep m_TS;

	Mortify::Ref<Mortify::Window> m_Window;

	Mortify::Ref<Mortify::Window> m_SecondWindow;
};