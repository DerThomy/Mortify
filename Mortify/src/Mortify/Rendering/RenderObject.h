#pragma once

#include "Feature/RenderComponent.h"

namespace Mortify
{
// Handle between Render and Gaming Thread
	class RenderObject
	{
	private:
		bool m_IsVisible : 1;
		RenderComponent* ComponentHandle;
	public:
		RenderObject(RenderComponent* component)
			: ComponentHandle(component) {}
	};
}