#pragma once

namespace Triton
{

	class RenderAction
	{
	public:
		virtual ~RenderAction() { }
		virtual void Execute() = 0;
	};

}