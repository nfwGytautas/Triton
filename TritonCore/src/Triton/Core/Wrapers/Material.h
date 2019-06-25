#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Material : public PType::PlatformWraper<PType::Texture>, public Resource::Asset
		{
		public:
			Material(reference<PType::Texture> obj);

			virtual ~Material()
			{ }

			// Inherited via PlatformWraper
			virtual void enable() override;

			reference<PType::Shader> Shader;
			Vector3 Ambient;
			Vector3 Diffuse;
			Vector3 Specular;
			float Shininess;
		};

	}
}