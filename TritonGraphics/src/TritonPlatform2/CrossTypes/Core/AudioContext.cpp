#include "AudioContext.h"

#include "Triton2/Utility/Log.h"

#ifdef TR_PLATFORM_WINDOWS
#include "TritonPlatform2/Windows/Core/WindowsAudioContext.h"
#define TR_RETURN_CONTEXT static_cast<Triton::Audio::AudioContext*>(new Triton::Audio::WindowsAudioContext())
#define TR_CONTEXT_TYPE "WINDOWS"
#endif

namespace Triton
{
	namespace Audio
	{
		AudioContext* AudioContext::create()
		{
			TR_SYSTEM_TRACE("Creating '{0}' audio context", TR_CONTEXT_TYPE);
			return TR_RETURN_CONTEXT;
		}

		Core::ThreadSynchronizer& AudioContext::synchronizer() const
		{
			return *m_synchronizer;
		}
	}
}