#include "TRpch.h"
#include "Timer.h"

namespace Triton
{
	namespace Utility
	{
		Timer::Timer(bool start)
		{
			if (start)
			{
				this->start();
			}
		}

		Timer::~Timer()
		{
			end();
		}

		void Timer::start()
		{
			m_start = std::chrono::system_clock::now();
		}

		void Timer::end()
		{
			m_end = std::chrono::system_clock::now();
			m_ended = true;
		}

		double Timer::elapsedTime() const
		{
			return (m_end - m_start).count();
		}
	}
}