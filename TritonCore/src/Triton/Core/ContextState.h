#pragma once

#include "Triton\Core\Shader\Shader.h"
#include "Triton\Core\Data\Mesh.h"

namespace Triton
{
	namespace Core
	{

		class ContextState
		{
		public:
			std::shared_ptr<Shader> BoundShader;
			std::shared_ptr<Data::Mesh> BoundMesh;
		};

	}
}