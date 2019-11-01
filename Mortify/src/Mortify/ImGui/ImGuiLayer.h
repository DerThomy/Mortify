#pragma once

#include "Mortify/Core/Layer.h"

#include "Mortify/Core/Events/ApplicationEvent.h"
#include "Mortify/Core/Events/MouseEvent.h"
#include "Mortify/Core/Events/KeyEvent.h"

namespace Mortify
{
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}