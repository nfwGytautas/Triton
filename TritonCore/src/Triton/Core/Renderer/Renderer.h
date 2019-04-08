#pragma once

#include "Triton\Core\Renderer\RenderRoutine.h"

namespace Triton
{
	namespace Core
	{

		class Renderer
		{
		public:
			virtual void Prepare() = 0;
			virtual void Render(unsigned int aIndiceCount) = 0;

			virtual ~Renderer() { }
		private:
		};

	}
}