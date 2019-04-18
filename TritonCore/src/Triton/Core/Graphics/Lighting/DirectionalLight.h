#pragma once
#include "Light.h"

namespace Triton
{
	namespace Graphics
	{

		class DirectionalLight : public Light
		{
		public:
			DirectionalLight();
			~DirectionalLight();
		};

	}
}