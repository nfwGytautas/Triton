#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Mesh : public Resource::Asset
		{
		public:
			Mesh(size_t assetID, reference<PType::VAO> object);

			virtual ~Mesh()
			{ }

			reference<PType::VAO> VAO;
		};

	}
}