#pragma once

#include "Mortify/Rendering/FramePacket.h"
#include "Mortify/Rendering/RenderObject.h"

namespace Mortify
{
	class RenderComponent
	{
	public:
		virtual void Extract(FramePacket &framePacket)
		{
		}

		RenderObject* GetRenderObject() const { return m_RenderObjectHandle; }

		void SetRenderObject(RenderObject* renderObject) { m_RenderObjectHandle = renderObject; }
	private:
		RenderObject* m_RenderObjectHandle;
	};
}