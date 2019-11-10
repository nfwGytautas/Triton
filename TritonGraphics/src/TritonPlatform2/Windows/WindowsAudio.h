#pragma once

#include <vector>

#include "TritonPlatform2/Windows/WindowsMacros.h"
#include "TritonPlatform2/CrossTypes/Audio.h"

namespace Triton
{
	namespace Audio
	{
		class WindowsAudioContext;

		class WindowsAudio : public Audio
		{
		public:
			virtual ~WindowsAudio();

			// Inherited via Audio
			virtual void play() override;
		private:
			IXAudio2SourceVoice* m_sourceVoice;
			std::vector<unsigned char> m_audioData;

			friend WindowsAudioContext;
		};
	}
}