#include "Shader.h"

#include "Triton/Logger/Log.h"

namespace Triton
{
	namespace Graphics
	{

		Shader::Shader()
		{
		}

		Shader::~Shader()
		{
		}

		MatrixBuffer& Shader::buffer_matrices()
		{
			return prtc_Matrices;
		}
	}
}