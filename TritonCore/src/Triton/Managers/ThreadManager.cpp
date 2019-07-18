#include "TRpch.h"
#include "ThreadManager.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "ThreadManager.h"

namespace Triton
{
	namespace Manager
	{
		void ThreadManager::newRenderFunc(RenderFunction renderFunction)
		{
			if (m_renderThread != nullptr)
			{
				m_renderThread->join();
				delete m_renderThread;
			}

			m_renderThread = new std::thread(renderFunction);
		}

		void ThreadManager::stopRendering()
		{
			if (m_renderThread != nullptr)
			{
				m_renderThread->join();
				delete m_renderThread;
			}
		}

	}
}