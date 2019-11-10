#include "WindowsAudio.h"

namespace Triton
{
	namespace Audio
	{
		WindowsAudio::~WindowsAudio()
		{
			m_sourceVoice->DestroyVoice();
		}

		void WindowsAudio::play()
		{
			m_sourceVoice->Start();
		}
	}
}