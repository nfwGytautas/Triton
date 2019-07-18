#include "TRpch.h"
#include "Utility.h"

namespace Triton
{
	namespace Utility
	{
		void Timer::onMessage(size_t message, void * payload)
		{
			switch (message)
			{
			case (size_t)Core::TritonMessageType::Render:
			{
				time now = m_timer.now();

				m_renderDelta = std::chrono::duration_cast<ms>(now - m_prevRenderTime).count() / 1000.0f;

				m_prevRenderTime = now;

				return;
			}
			case (size_t)Core::TritonMessageType::Update:
			{
				time now = m_timer.now();

				m_updateDelta = std::chrono::duration_cast<ms>(now - m_prevUpdateTime).count() / 1000.0f;

				m_prevUpdateTime = now;

				return;
			}
			}
		}
	}
}