#pragma once
#include "Triton\TRMacros.h"
#include "TritonTypes/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Material : public PType::PlatformWraper<PType::Texture>, public Resource::Asset
		{
		public:
			Material(PType::Texture* obj);

			virtual ~Material()
			{ }

			// Inherited via PlatformWraper
			virtual void enable() override;

			relay_ptr<PType::Shader> Shader;
			Vector3 Ambient;
			Vector3 Diffuse;
			Vector3 Specular;
			float Shininess;

		protected:
			// Inherited via Asset
			virtual void destroyFunc(relay_ptr<PType::Context> context, PType::FactoryDestroyParams* params) override;
		};

	}
}