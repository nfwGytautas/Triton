#pragma once
#include "Triton\TRMacros.h"
#include "Triton\Core\Data\Structures\RenderBatch.h"
#include "Triton\Entity\Registry.h"
#include "Triton\Core\Shader\Shader.h"

namespace Triton
{
	namespace Core
	{

		class TRITON_API Renderer
		{
		public:
			static Renderer* Create(Shader* aShader);

			virtual ~Renderer() {}

			virtual void Render(std::vector<Data::RenderBatch>& aRenderBatch) = 0;
		private:
		};

	}

}