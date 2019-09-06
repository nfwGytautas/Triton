#include "TRpch.h"
#include "ThreadManager.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "ThreadManager.h"

#include <thread>

namespace Triton
{
	namespace Manager
	{
		class ThreadManager::Impl
		{
		public:
			std::thread* m_renderThread;
		};

		void ThreadManager::newRenderFunc(RenderFunction renderFunction)
		{
			if (m_impl->m_renderThread != nullptr)
			{
				m_impl->m_renderThread->join();
				delete m_impl->m_renderThread;
			}

			m_impl->m_renderThread = new std::thread(renderFunction);
		}

		void ThreadManager::stopRendering()
		{
			if (m_impl->m_renderThread != nullptr)
			{
				m_impl->m_renderThread->join();
				delete m_impl->m_renderThread;
			}
		}

	}
}