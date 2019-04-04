#pragma once

#include "RenderObject.h"
#include "RenderNode.h"

class ViewPacket;

namespace Mortify
{
	class FramePacket
	{
	private:
		std::vector<ViewPacket> m_ViewPackets;
		std::vector<RenderNode> m_RenderNodes;

	public:
		void AddViewPacket(ViewPacket& viewPacket);

		// Add extracted Render Object data
		template <typename T>
		void AddExtractedRenderObjectData(RenderObject* renderObject, const T& extractedData)
		{
			MT_CORE_INFO("Extracted Render Object data: {0}", extractedData);
		}

		bool HasExtracted(RenderObject* renderObject) const
		{
			return false;
		}

		void Extract();
	};
}