#pragma once

#include "Mortify/Layer.h"

#include "Mortify/Events/ApplicationEvent.h"
#include "Mortify/Events/MouseEvent.h"
#include "Mortify/Events/KeyEvent.h"

namespace Mortify
{
	class MT_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}