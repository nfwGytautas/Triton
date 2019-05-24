#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"

#include "TritonPlatform\PlatformAbstraction.h"

namespace Triton
{
	namespace Data
	{
		
		class  File
		{
		public:
			static std::string ReadAllText(std::string& aPath);
			static bool Valid(std::string& aPath);

			static void ReadMesh(std::string aPath, Triton::PType::VAOCreateParams* params);
			static void ReadTexture(std::string aPath, Triton::PType::TextureCreateParams* params);
		};

	}
}