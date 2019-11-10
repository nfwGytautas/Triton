#pragma once

#include "TritonPlatform2/Windows/WindowsMacros.h"
#include "TritonPlatform2/CrossTypes/Core/AudioContext.h"

namespace Triton
{
	namespace Audio
	{
		class WindowsAudioContext : public AudioContext
		{
		public:
			// Inherited via AudioContext
			virtual bool init(EngineSettings& settings) override;
			virtual void shutdown() override;
			virtual Audio* newAudio(const IO::AudioData & createParams) override;
		private:
			IXAudio2* m_XAudio2;
			IXAudio2MasteringVoice* m_masteringVoice;			
		};
	}
}