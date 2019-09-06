#pragma once

#include "TritonTypes\relay_ptr.h"
#include "Triton\State\State.h"

namespace Triton
{
	namespace Interface
	{
		class IStateChanger
		{
		public:
			virtual ~IStateChanger() { }

			virtual void Bind(relay_ptr<Singleton::State> aState) = 0;
			virtual void Unbind(relay_ptr<Singleton::State> aState) = 0;
		};
	}
}