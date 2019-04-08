#pragma once

#include "Triton\Core\Shader\Shader.h"

namespace Triton
{
	namespace Core
	{

		class UniformHost
		{
		public:
			void AddUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform);
			void SetUniforms(Shader& aShader);
		private:
			std::vector<std::shared_ptr<ShaderUniforms::ShaderUniform>> m_Uniforms;
		};

	}
}