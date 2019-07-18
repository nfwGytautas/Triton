#pragma once

#include "Triton/Core/TritonClass.h"

#include <chrono>

namespace Triton
{
	namespace Utility
	{
		// Utility class that calculates the time passed between render and update messages
		class Timer : public Core::TritonClass
		{
		public:
			virtual void onMessage(size_t message, void* payload) override;

			float updateDelta() const
			{
				return m_updateDelta;
			}

			float renderDelta() const
			{
				return m_renderDelta;
			}
		private:
			using ms = std::chrono::duration<float, std::milli>;
			using time = std::chrono::time_point<std::chrono::steady_clock>;
			
			std::chrono::high_resolution_clock m_timer;

			// The delta for an update cycle
			float m_updateDelta = 0.0f;
			// The delta for a render cycle
			float m_renderDelta = 0.0f;

			// The time the previous render message was sent
			time m_prevRenderTime;

			// The time the previous update message was sent
			time m_prevUpdateTime;
		};
	}
}