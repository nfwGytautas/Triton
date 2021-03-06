#pragma once

#include <chrono>

namespace Triton
{
	namespace Utility
	{
		/**
		 * Utility class for keeping track of time
		 */
		class Timer
		{
		public:
			/**
			 * Create a timer instance and specify if it should start keeping time
			 *
			 * @param start If true then the timer will immediately start else start()
			 * will need to be called
			 */
			Timer(bool start);
			~Timer();

			/**
			 * Starts the timer
			 */
			void start();

			/**
			 * Ends the timer
			 */
			void end();

			/**
			 * Get the duration that has elapsed between the start
			 * of the timer and the end of the timer in microseconds
			 *
			 * @return The amount of time that has elapsed since start in microseconds
			 */
			double microseconds() const;

			/**
			 * Get the duration that has elapsed between the start
			 * of the timer and the end of the timer in milliseconds
			 *
			 * @return The amount of time that has elapsed since start in milliseconds
			 */
			double milliseconds() const;

			/**
			 * Get the duration that has elapsed between the start
			 * of the timer and the end of the timer in seconds
			 *
			 * @return The amount of time that has elapsed since start in seconds
			 */
			double seconds() const;
		private:
			/// Time point of Timer start
			std::chrono::time_point<std::chrono::system_clock> m_start;
			/// Time point of Timer end
			std::chrono::time_point<std::chrono::system_clock> m_end;

			/// Variable to keep track if Timer has been ended by hand
			bool m_ended = false;
		};
	}
}