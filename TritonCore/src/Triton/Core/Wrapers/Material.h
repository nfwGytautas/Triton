#pragma once
#include "Triton\TRMacros.h"
#include "TritonPlatform/mathematical.h"

#include "TritonPlatform/PlatformAbstraction.h"
#include "Triton/Core/Asset/Asset.h"

namespace Triton
{
	namespace Data
	{
		class Material : public Resource::Asset
		{
		public:
			Material(size_t assetID, reference<PType::Texture> object);

			virtual ~Material()
			{ }

			reference<PType::Texture> Texture;
			reference<PType::Shader> Shader;
			Vector3 Ambient;
			Vector3 Diffuse;
			Vector3 Specular;
			float Shininess;
		};

	}
}