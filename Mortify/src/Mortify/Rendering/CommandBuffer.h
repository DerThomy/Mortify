#pragma once

#include "mtpch.h"

#include "RenderCommand.h"

namespace Mortify
{
	struct CmdClear : public RenderCommand
	{
		Color m_Color;

		CmdClear(const Color& color) : m_Color(color) {}

		void Execute(RenderContext* context) override final
		{
			context->ClearBuffer(m_Color);
		}
	};

	struct CmdPresent : public RenderCommand
	{
		void Execute(RenderContext* context) override final
		{
			context->Present();
		}
	};

	volatile bool IsRenderingThread() { return true; }

	class CommandBuffer : public RenderCommand
	{
	private:
		std::vector<RenderCommand*> m_Commands;
	public:
		void AddCommand(RenderCommand* command)
		{
			m_Commands.push_back(command);
		}

		void Clear(const Color& color)
		{
			m_Commands.push_back(new CmdClear(color));
		}

		void Present()
		{
			m_Commands.push_back(new CmdPresent());
		}

		void Execute(RenderContext* context) override final
		{
			if (!IsRenderingThread())
				throw;

			for (int i = 0; i < m_Commands.size(); i++)
			{
				m_Commands[i]->Execute(context);
			}
		}
	};
}