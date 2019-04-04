#pragma once

#include "mtpch.h"

#include "RenderObject.h"

namespace Mortify
{
	class ViewPacket
	{
		std::vector<RenderObject*> m_RenderObjects;

		void AddRenderObject(RenderObject* renderObject);

		void ExtractObjects(FramePacket& framePacket);
	};
}