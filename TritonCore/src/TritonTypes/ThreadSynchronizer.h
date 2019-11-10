#pragma once

#include <functional>

namespace Triton
{
	namespace Core
	{
		/**
		* Class used to allow non main threads to queue jobs to do on another thread.
		* Synchronizer keeps a queue of jobs and on the synchronize step of the main loop 
		* executes specified amount of queued jobs
		*/
		class ThreadSynchronizer
		{
		public:
			ThreadSynchronizer();
			~ThreadSynchronizer();

			/**
			 * Add an action to synchronize for this synchronizer
			 * 
			 * @param action std::function that returns void and has no parameters
			 */
			void enqueue(std::function<void()> action);

			/**
			 * Synchronizes enqueued actions by calling them on this thread
			 * The amount of actions to synchronize is determined by the amount parameter
			 * The synchronizer will not synchronize more than the specified amount, however
			 * it can synchronize less if the queue is empty
			 * If amount is 0 the synchronizer will synchronize all actions
			 *
			 * @param amount the amount of actions that can be synchronized this call
			 */
			void synchronize(unsigned int amount);
		private:
			class ThreadSynchronizerImpl;
			ThreadSynchronizerImpl* m_impl;
		};
	}
}