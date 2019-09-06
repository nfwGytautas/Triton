#include "TRpch.h"
#include "Material.h"

Triton::Data::Material::Material(size_t assetID, reference<PType::Texture> object)
	: Asset(assetID), Texture(object), Ambient(1.0f, 1.0f, 1.0f), Diffuse(1.0f, 1.0f, 1.0f), Specular(1.0f, 1.0f, 1.0f), Shininess(32.0f)
{
}
