#pragma once

#include "RenderAction.h"

namespace Triton
{
	namespace Core
	{

		class Renderer
		{
		public:
			Renderer();

			void Render();

			template <class T, class... _Args>
			void AddAction(_Args&&... Args)
			{
				m_Actions.push(std::make_shared<T>(std::forward<_Args>(Args)...));
			}
		private:
			std::queue<std::shared_ptr<RenderAction>> m_Actions;
		};

	}
}