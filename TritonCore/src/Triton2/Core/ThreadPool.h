#pragma once

#include <thread>
#include <atomic>
#include <future>

namespace Triton
{
	namespace Core
	{
		namespace Thread
		{
			/**
			 * Class used to manage multiple worker threads and a single task queue
			 *
			 * One queue per pool
			 */
			class ThreadPool
			{
			public:
				/**
				 * Creates a ThreadPool with max amount of threads that are allowed by the system
				 */
				explicit ThreadPool();

				/**
				 * Creates a ThreadPool with specified thread count
				 *
				 * @param threadCount The amount of threads inside the pool
				 */
				explicit ThreadPool(size_t threadCount);

				/**
				 * Waits for all threads to finish and then cleans up
				 */
				~ThreadPool();

				/**
				 * Add a task to the pool queue
				 *
				 * @param task A task that takes nothing and returns void
				 */
				template<class F>
				void enqueue(F&& task)
				{
					std::unique_lock<std::mutex> lock(m_queueMutex);
					m_tasks.emplace_back(std::forward<F>(task));
					m_cvTask.notify_one();
				}

				/**
				 * Wait for all workers inside this thread pool
				 */
				void wait();

				/**
				 * Check the state of the thread pool
				 *
				 * @return True if the ThreadPool is stopped, false otherwise
				 */
				bool isStopped() const;
			private:
				/**
				 * Function ran by all pool threads
				 */
				void threadProcess();

				/**
				 * Stops the ThreadPool entirely
				 */
				void terminate();

				/// Vector of workers created by this pool
				std::vector<std::thread> m_threads;

				/// Queue of tasks
				std::deque<std::function<void()>> m_tasks;

				/// Mutex for keeping the thread pool thread safe
				mutable std::mutex m_queueMutex;

				/// Condition variable to notify task queue changes
				std::condition_variable m_cvTask;

				/// Condition variable to notify task completion
				std::condition_variable m_cvFinished;

				/// Variable to keep track of how many tasks are being executed right now
				unsigned int m_runningTasks;

				/// Boolean to check if the thread pool is terminated or not
				bool m_terminated;
			};
		}
	}
}