#pragma once

#include "Mortify/Define.h"
#include "Mortify/Core/Timestep.h"
#include "Mortify/Events/Event.h"
#include "Mortify/Window.h"

namespace Mortify
{
	class MT_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}