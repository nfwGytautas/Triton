#pragma once

#include "TritonTypes/reference.h"
#include "TritonTypes/Settings.h"

#include "TritonTypes/IO.h"

#include "TritonPlatform2/CrossTypes/Audio.h"

#include "TritonTypes/ThreadSynchronizer.h"

namespace Triton
{
	namespace Audio
	{
		/**
		 * This class is a the same as the graphics Context except for Audio.
		 * The audio context is used to provide cross platform audio playback and creation.
		 */
		class AudioContext
		{
		public:
			virtual ~AudioContext() { }

			/**
			 * Create the context
			 *
			 * @return The API specific context cast to AudioContext base class
			 */
			static AudioContext* create();

			/**
			 * Initialize the Context class
			 *
			 * @param settings Engine settings passed from engine init method
			 * @return Boolean that specifies if the initialization was successful
			 */
			virtual bool init(EngineSettings& settings) = 0;

			/**
			 * Shutdown the context and clean all the resources
			 */
			virtual void shutdown() = 0;

			/**
			 * Create a new audio object
			 *
			 * @param createParams The parameters of the audio object
			 * @return Pointer to the newly created audio object
			 */
			virtual Audio* newAudio(const IO::AudioData& createParams) = 0;

			/**
			 * Returns the ThreadSynchronizer of the context
			 *
			 * @return ThreadSynchronizer associated with current context
			 */
			Core::ThreadSynchronizer& synchronizer() const;
		protected:
			/// Variable to track if the context has been initialized or not
			bool m_initialized = false;

			/// Thread synchronizer used by the audio context
			Core::ThreadSynchronizer* m_synchronizer;
		};
	}
}