#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Entity\Registry.h"
#include "Triton\Core\Shader\Shader.h"
#include "Triton\Entity\Registry.h"

namespace Triton
{
	namespace Core
	{

		class TRITON_API Renderer
		{
		public:
			static Renderer* Create(Shader* aShader);

			virtual ~Renderer() {}

			virtual void Render(ECS::Registry& aRegistry) = 0;
		private:
		};

	}

}