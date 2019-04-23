#pragma once
#include "RenderAction.h"
#include "Triton\Core\ContextState.h"

namespace Triton
{

	class RenderChain
	{
	public:
		RenderChain() {}
		virtual ~RenderChain() {}

		template <class T, class... _Args>
		void AddAction(_Args&&... Args)
		{
			m_Actions.push(std::make_shared<T>(std::forward<_Args>(Args)...));
		}

		void Execute(Core::ContextState& aState);
	private:
		std::queue<std::shared_ptr<RenderAction>> m_Actions;
	};

}