#pragma once

#include "TritonTypes/reference.h"

namespace Triton
{
	namespace Graphics
	{
		// Base class for most platform objects
		class PlatformObject
		{
		public:
			virtual ~PlatformObject() { }
		};

		// Base class for renderable objects
		class Renderable : public PlatformObject
		{
		public:
			virtual ~Renderable() { }

			virtual unsigned int getIndiceCount() = 0;
		};
	}
}