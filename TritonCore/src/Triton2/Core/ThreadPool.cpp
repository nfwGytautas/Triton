#include "TRpch.h"
#include "ThreadPool.h"

#include "Triton2/Utility/Log.h"

namespace Triton
{
	namespace Core
	{
		namespace Thread
		{
			WorkerThread::WorkerThread()
				: m_terminated(false)
			{
				m_thread = std::thread(std::bind(&WorkerThread::process, this));
			}

			WorkerThread::~WorkerThread()
			{
				terminate();
			}

			void WorkerThread::wait()
			{
				while (isRunning())
				{ }
			}

			void WorkerThread::terminate()
			{
				m_terminated = true;
				m_thread.join();
			}

			void WorkerThread::addTask(std::function<void()> task)
			{
				std::lock_guard<std::mutex> guard(m_queueLock);

				if (m_terminated)
				{
					return;
				}

				m_tasks.push(task);
				m_cv.notify_one();
			}

			bool WorkerThread::isRunning() const
			{
				return !m_tasks.empty();
			}

			size_t WorkerThread::taskCount() const
			{
				return m_tasks.size();
			}

			void WorkerThread::process()
			{
				for (;;)
				{
					if (m_terminated)
					{
						return;
					}

					auto task = takeTask();
					task();
				}
			}

			std::function<void()> WorkerThread::takeTask()
			{
				std::unique_lock<std::mutex> guard(m_queueLock);
				m_cv.wait(guard, [&] { return !m_tasks.empty(); });

				auto task = std::move(m_tasks.front());
				m_tasks.pop();

				return task;
			}


			ThreadPool::ThreadPool()
				: m_size(std::thread::hardware_concurrency()),
				m_stopped(false)
			{
				createPool();
			}

			ThreadPool::ThreadPool(std::size_t threadCount)
				: m_size(threadCount),
				m_stopped(false)
			{
				createPool();
			}

			ThreadPool::~ThreadPool()
			{
				wait();
				terminate();
			}

			void ThreadPool::run(std::function<void()> task)
			{
				size_t worker = getWorker();
				m_workers[worker]->addTask(std::move(task));
			}

			size_t ThreadPool::getWorker()
			{
				// Default add to 0 thread
				size_t idLeast = 0;
				size_t tasks = m_workers[0]->taskCount();

				for (size_t i = 1; i < m_workers.size(); i++)
				{
					size_t taskCount = m_workers[i]->taskCount();
					if (tasks > taskCount)
					{
						idLeast = i;
						tasks = taskCount;
					}
				}

				return idLeast;
			}

			void ThreadPool::wait()
			{
				std::lock_guard<decltype(m_lock)> pl(m_lock);
				for (const auto& w : m_workers)
				{
					w->wait();
				}
			}

			void ThreadPool::terminate()
			{
				std::lock_guard<decltype(m_lock)> pl(m_lock);
				// Join everything
				for (auto& w : m_workers)
				{
					w->terminate();
				}
			}

			void ThreadPool::createPool()
			{
				std::lock_guard<decltype(m_lock)> pl(m_lock);
				for (std::size_t i = 0; i < m_size; i++)
				{
					m_workers.push_back(std::make_unique<WorkerThread>());
				}
			}

			bool ThreadPool::isStopped() const
			{
				return m_stopped;
			}
		}
	}
}