#pragma once

#include "Triton\Core\ContextState.h"

namespace Triton
{

	class RenderAction
	{
	public:
		virtual ~RenderAction() { }
		virtual void Execute(Core::ContextState& aState) = 0;
	};

}