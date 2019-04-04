#include "mtpch.h"

#include "FramePacket.h"

namespace Mortify
{
	void FramePacket::AddViewPacket(ViewPacket& viewPacket)
	{
		m_ViewPackets.push_back(viewPacket);
	}

	void FramePacket::Extract()
	{
		for (int i = 0; i < m_ViewPackets.size(); i++)
		{
			m_ViewPackets[i].ExtractObject(*this);
		}
	}
}