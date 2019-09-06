#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class PlainTexture : public Resource::Asset
		{
		public:
			PlainTexture(size_t assetID, reference<PType::Texture> object);

			virtual ~PlainTexture()
			{ }

			reference<PType::Texture> Texture;
		};

	}
}