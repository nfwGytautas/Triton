#pragma once

#include "Triton\Core\ContextState.h"
#include "Triton\Core\Renderer\RenderChain.h"

namespace Triton
{
	namespace Core
	{

		class Renderer : private RenderChain
		{
		public:
			Renderer();

			void Render();

			using RenderChain::AddAction;
		private:
			std::shared_ptr<ContextState> m_State;
		};

	}
}