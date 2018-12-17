#pragma once
#include "TRpch.h"

#include "Triton\TRMacros.h"

namespace Triton
{
	namespace Core
	{
		namespace GraphicsAPI
		{
			enum class ShaderType
			{
				Vertex,
				Fragment,
			};

			TRITON_API bool Initialize();

			TRITON_API void ChangeViewport(double aWidth, double aHeight);

			TRITON_API unsigned int CreateShader(ShaderType aShaderType, std::string& aShaderSource);
			TRITON_API unsigned int CreateProgram(std::initializer_list<unsigned int> aShaders);

			TRITON_API unsigned int GetShaderAttributeLocation(const unsigned int& aProgramID, const std::string& aAttributeName);
		}
	}
}