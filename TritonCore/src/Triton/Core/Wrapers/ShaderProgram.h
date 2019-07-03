#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class ShaderProgram : public Resource::Asset
		{
		public:
			ShaderProgram(size_t assetID, reference<PType::Shader> object);

			virtual ~ShaderProgram()
			{ }

			reference<PType::Shader> Program;
		};

	}
}