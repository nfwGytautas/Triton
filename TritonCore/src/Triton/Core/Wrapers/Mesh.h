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
			Mesh(PType::VAO* obj);

			virtual ~Mesh()
			{ }

			// Inherited via PlatformWraper
			virtual void enable() override;

		private:
			// Inherited via Asset
			virtual void destroyFunc(relay_ptr<PType::Context> context, PType::FactoryDestroyParams * params) override;
		};

	}
}