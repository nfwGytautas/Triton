#include "TRpch.h"
#include "ThreadPool.h"

#include "Triton2/Utility/Log.h"

namespace Triton
{
	namespace Core
	{
		namespace Thread
		{
			ThreadPool::ThreadPool()
				: ThreadPool(std::thread::hardware_concurrency())
			{
				
			}

			ThreadPool::ThreadPool(size_t threadCount)
				: m_runningTasks(), m_terminated()
			{
				for (unsigned int i = 0; i < threadCount; ++i)
				{
					m_threads.emplace_back(std::bind(&ThreadPool::threadProcess, this));
				}
			}

			ThreadPool::~ThreadPool()
			{
				std::unique_lock<std::mutex> latch(m_queueMutex);
				m_terminated = true;
				m_cvTask.notify_all();
				latch.unlock();

				for (auto& t : m_threads)
					t.join();
			}

			void ThreadPool::wait()
			{
				std::unique_lock<std::mutex> lock(m_queueMutex);
				m_cvFinished.wait(lock, [this]() { return m_tasks.empty() && (m_runningTasks == 0); });
			}

			bool ThreadPool::isStopped() const
			{
				return m_terminated;
			}

			void ThreadPool::threadProcess()
			{
				while (true)
				{
					std::unique_lock<std::mutex> latch(m_queueMutex);
					m_cvTask.wait(latch, [this]() { return m_terminated || !m_tasks.empty(); });
					if (!m_tasks.empty())
					{
						++m_runningTasks;

						auto fn = m_tasks.front();
						m_tasks.pop_front();

						// Async
						latch.unlock();
						fn();
						latch.lock();

						--m_runningTasks;
						m_cvFinished.notify_one();
					}
					else if (m_terminated)
					{
						break;
					}
				}
			}

			void ThreadPool::terminate()
			{
				m_terminated = true;
			}


		}
	}
}