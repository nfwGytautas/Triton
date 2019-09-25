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
			 * A class that wraps std::thread and a std::queue to create a wrapper
			 * and allow for parallel tasking
			 */
			class WorkerThread
			{
			public:
				/**
				 * Create a WorkerThread instance
				 * Starts the thread automatically as soon as it gets created
				 */
				WorkerThread();

				/**
				 * Wait for the thread and join it and then destroy WorkerThread instance
				 */
				~WorkerThread();

				/**
				 * Wait until there is no longer any work to be done
				 * Does not join the thread
				 */
				void wait();

				/**
				 * Wait for work to be complete and then join the thread
				 */
				void terminate();

				/**
				 * Add a task for this worker
				 *
				 * @param task A function that return nothing and takes nothing as a task
				 */
				void addTask(std::function<void()> task);

				/**
				 * Check if this worker has work to do e.g. tasks queue isn't empty
				 * 
				 * @return True if queue isn't empty, false otherwise
				 */
				bool isRunning() const;

				/**
				 * Get the amount of tasks left for this worker
				 *
				 * @return Amount of tasks inside the queue
				 */
				size_t taskCount() const;
			private:
				/**
				 * The inner loop of the std::thread
				 */
				void process();

				/**
				 * Take a task from queue and return it
				 *
				 * @return Task to be executed
				 */
				std::function<void()> takeTask();
			private:
				/// Atomic variable to keep track if the worker has been terminated or not
				std::atomic<bool> m_terminated;

				/// Underlying thread object
				std::thread m_thread;

				/// Condition variable used to notify the worker about changes to queue
				std::condition_variable m_cv;

				/// Mutex to make sure that no race conditions or deadlocks appear when interacting with queue
				mutable std::mutex m_queueLock;

				/// Task queue
				std::queue<std::function<void()>> m_tasks;
			};


			/**
			 * Class used to manage multiple worker threads
			 *
			 * One queue per worker
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
				 * Run a task on the thread pool
				 *
				 * @param task A function that takes nothing and returns nothing
				 * @return Future of the task
				 */
				void run(std::function<void()> task);

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
				 * Get the worker for who the pool should assign the task to
				 *
				 * @return ID of the worker to add the task to
				 */
				size_t getWorker();

				/**
				 * Creates a threadCount amount of threads for the pool
				 */
				void createPool();

				/**
				 * Stops the ThreadPool entirely
				 */
				void terminate();

				/// Vector of workers created by this pool
				std::vector<std::unique_ptr<WorkerThread>> m_workers;

				/// Mutex for keeping the thread pool thread safe
				mutable std::mutex m_lock;

				/// Variable to check if the pool has been stopped
				std::atomic<bool> m_stopped;

				/// Size of the pool.
				const std::size_t m_size;
			};
		}
	}
}