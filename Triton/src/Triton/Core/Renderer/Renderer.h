#pragma once

#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Core
	{

		class TRITON_API Renderer
		{
		public:
			Renderer() {}
			virtual ~Renderer() {}

			virtual void Render() = 0;
		private:
		};

	}
}