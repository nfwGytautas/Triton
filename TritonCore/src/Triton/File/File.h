#pragma once
#include "TRpch.h"
#include "Triton\TRMacros.h"

#include "Triton\Core\Data\Mesh.h"
#include "Triton\Core\Data\Material.h"

namespace Triton
{
	namespace Data
	{
		
		class  File
		{
		public:
			static std::string ReadAllText(std::string& aPath);
			static bool Valid(std::string& aPath);

			static std::shared_ptr<MeshData> ReadMesh(std::string aPath);
			static std::shared_ptr<TextureData> ReadTexture(std::string aPath);
		};

	}
}