#pragma once

#include "Triton\State\State.h"

namespace Triton
{

	class RenderAction
	{
	public:
		virtual ~RenderAction() { }
		virtual void Execute(relay_ptr<Singleton::State> aState) = 0;
	};

}