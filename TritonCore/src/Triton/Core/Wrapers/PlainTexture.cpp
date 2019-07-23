#include "TRpch.h"
#include "PlainTexture.h"

Triton::Data::PlainTexture::PlainTexture(size_t assetID, Triton::reference<PType::Texture> object)
	: Asset(assetID), Texture(object)
{
}