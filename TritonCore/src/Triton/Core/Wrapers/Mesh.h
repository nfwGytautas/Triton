#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Mesh : public PType::PlatformWraper<PType::VAO>, public Resource::Asset
		{
		public:
			Mesh(reference<PType::VAO> obj);

			virtual ~Mesh()
			{ }

			// Inherited via PlatformWraper
			virtual void enable() override;
		};

	}
}