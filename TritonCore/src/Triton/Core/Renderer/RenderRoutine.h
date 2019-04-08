#pragma once

#include "Triton\Core\Shader\UniformHost.h"

namespace Triton
{
	namespace Core
	{

		class RenderRoutine : public UniformHost
		{
		public:
			void Bind(std::shared_ptr<Core::Shader> aShader);

			//Specific rendering settings for a mesh to be added here

		};

	}
}