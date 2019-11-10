#include "WindowsAudioContext.h"

#include "Triton2/Utility/Log.h"

#include "TritonPlatform2/Windows/WindowsAudio.h"

namespace Triton
{
	namespace Audio
	{
		bool WindowsAudioContext::init(EngineSettings& settings)
		{
			TR_SYSTEM_TRACE("Initializing WindowsAudioContext");

			HRESULT result;

			CoInitializeEx(nullptr, COINIT_MULTITHREADED);

			result = XAudio2Create(&m_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create XAudio2");
				return false;
			}

			result = m_XAudio2->CreateMasteringVoice(&m_masteringVoice);
			if (FAILED(result))
			{
				TR_SYSTEM_ERROR("Failed to create IXAudio2MasteringVoice");
				return false;
			}

#ifdef TR_DEBUG
			XAUDIO2_DEBUG_CONFIGURATION debug = {};
			debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
			debug.BreakMask = XAUDIO2_LOG_ERRORS;
			m_XAudio2->SetDebugConfiguration(&debug, nullptr);
#endif
			m_synchronizer = new Core::ThreadSynchronizer();
			m_initialized = true;

			return true;
		}

		void WindowsAudioContext::shutdown()
		{
			TR_SYSTEM_WARN("Shutting down WindowsAudioContext");

			if (m_masteringVoice)
			{
				m_masteringVoice->DestroyVoice();
				m_masteringVoice = nullptr;
			}

			if (m_XAudio2)
			{
				m_XAudio2->Release();
				m_masteringVoice = nullptr;
			}

			delete m_synchronizer;

			CoUninitialize();
		}

		Audio* WindowsAudioContext::newAudio(const IO::AudioData& createParams)
		{
			TR_SYSTEM_TRACE("Creating a windows audio object of '{0}' format", createParams.FormatString);

			WindowsAudio* audio = new WindowsAudio();

			WAVEFORMATEX wfx = {0};
			XAUDIO2_BUFFER buffer = {0};
			HRESULT result;

			if (createParams.FormatString == "wav")
			{
				// Set the wave format of secondary buffer that this wave file will be loaded onto.
				wfx.wFormatTag = createParams.Format;
				wfx.nSamplesPerSec = createParams.SampleRate;
				wfx.wBitsPerSample = createParams.BitsPerSample;
				wfx.nChannels = createParams.Channels;
				wfx.nBlockAlign = createParams.BlockAllign;
				wfx.nAvgBytesPerSec = createParams.BytesPerSecond;
				wfx.cbSize = 0;
			}
			else
			{
				delete audio;
				TR_SYSTEM_ERROR("Invalid audio format");
				return nullptr;
			}

			// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
			audio->m_audioData = createParams.Data;

			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.AudioBytes = audio->m_audioData.size();
			buffer.pAudioData = audio->m_audioData.data();

			result = m_XAudio2->CreateSourceVoice(&audio->m_sourceVoice, &wfx);
			if (FAILED(result))
			{
				delete audio;
				TR_SYSTEM_ERROR("Creating source voice failed");
				return nullptr;
			}

			result = audio->m_sourceVoice->SubmitSourceBuffer(&buffer);
			if (FAILED(result))
			{
				delete audio;
				TR_SYSTEM_ERROR("Submitting source buffer failed");
				return nullptr;
			}

			TR_SYSTEM_INFO("Audio object created");

			return audio;
		}
	}
}