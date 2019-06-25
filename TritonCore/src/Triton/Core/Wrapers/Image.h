#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Image : public PType::PlatformWraper<PType::Bitmap>, public Resource::Asset
		{
		public:
			Image(reference<PType::Bitmap> obj);

			virtual ~Image()
			{ }

			// Inherited via PlatformWraper
			virtual void enable() override;
		};

	}
}