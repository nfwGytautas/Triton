#pragma once
#include "RenderAction.h"

namespace Triton
{

	class RenderChain
	{
	public:
		template <class T, class... _Args>
		void AddAction(_Args&&... Args)
		{
			m_Actions.push(std::make_shared<T>(std::forward<_Args>(Args)...));
		}

		void Execute();
	private:
		std::queue<std::shared_ptr<RenderAction>> m_Actions;
	};

}