#pragma once

#include <thread>

#include "Triton/Core/TritonClass.h"

namespace Triton
{
	namespace Manager
	{
		class ThreadManager : public Core::TritonClass
		{
		public:
			using RenderFunction = std::function<void()>;

			// Starts a rendering function on a new thread
			void newRenderFunc(RenderFunction renderFunction);

			// Stops the rendering thread
			void stopRendering();
		private:
			std::thread* m_renderThread;
		};
	}
}