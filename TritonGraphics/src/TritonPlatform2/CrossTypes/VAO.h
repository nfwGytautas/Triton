#pragma once

#include "TritonPlatform2/mathematical.h"
#include "TritonPlatform2/CrossTypes/Base.h"

namespace Triton
{
	namespace Graphics
	{
		class VAO : public Renderable
		{
		public:
			virtual ~VAO() { }

			virtual void enable() = 0;
			virtual void disable() = 0;

			virtual unsigned int getIndiceCount() = 0;
		protected:
			VAO()
			{ }
		};

	}
}