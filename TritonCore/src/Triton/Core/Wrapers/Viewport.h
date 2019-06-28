#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Viewport : public Resource::Asset
		{
		public:
			Viewport(size_t assetID, reference<PType::Framebuffer> object);

			unsigned int Width;
			unsigned int Height;

			virtual ~Viewport()
			{ }

			reference<PType::Framebuffer> Framebuffer;
		};

	}
}