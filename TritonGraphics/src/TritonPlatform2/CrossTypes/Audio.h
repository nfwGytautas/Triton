#pragma once

namespace Triton
{
	namespace Audio
	{
		/**
		 * Class that provides interface for playing an audio file
		 * Created using the AudioContext
		 */
		class Audio
		{
		public:
			virtual ~Audio() { }

			/**
			 * Starts playing audio that was used when creating the object
			 */
			virtual void play() = 0;
		protected:
			Audio()
			{ }
		};
	}
}