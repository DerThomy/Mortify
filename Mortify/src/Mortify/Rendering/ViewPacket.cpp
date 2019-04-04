#include "mtpch.h"

#include "ViewPacket.h"

namespace Mortify
{
	void ViewPacket::AddRenderObject(RenderObject* renderObject)
	{
		MT_CORE_ASSERT(renderObject, "AddRenderObject: renderObject null")
		m_RenderObjects.push_back(renderObject);
	}

	void ViewPacket::ExtractObjects(FramePacket& framePacket)
	{
		
	}
}