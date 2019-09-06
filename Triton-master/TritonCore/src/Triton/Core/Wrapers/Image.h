#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Image : public Resource::Asset
		{
		public:
			Image(size_t assetID, reference<PType::Bitmap> object);

			virtual ~Image()
			{ }

			reference<PType::Bitmap> Bitmap;
		};

	}
}