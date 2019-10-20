#include "TRpch.h"
#include "ThreadSynchronizer.h"

#include <thread>
#include <mutex>

namespace Triton
{
	namespace Core
	{
		class ThreadSynchronizer::ThreadSynchronizerImpl
		{
		public:
			void enqueue(std::function<void()> action)
			{
				std::lock_guard guard(m_mtx);
				m_queue.push(action);
			}

			void synchronize(unsigned int amount)
			{
				std::lock_guard guard(m_mtx);
				
				if (amount == 0 || amount > m_queue.size())
				{
					amount = m_queue.size();
				}

				for (unsigned int i = 0; i < amount; i++)
				{
					auto& task = m_queue.front();
					task();
					m_queue.pop();
				}
			}
		private:
			mutable std::mutex m_mtx;
			std::queue<std::function<void()>> m_queue;
		};

		ThreadSynchronizer::ThreadSynchronizer()
			: m_impl(new ThreadSynchronizerImpl())
		{
		}

		ThreadSynchronizer::~ThreadSynchronizer()
		{
			delete m_impl;
		}

		void ThreadSynchronizer::enqueue(std::function<void()> action)
		{
			m_impl->enqueue(action);
		}

		void ThreadSynchronizer::synchronize(unsigned int amount)
		{
			m_impl->synchronize(amount);
		}
	}
}