#include "TRpch.h"
#include "Image.h"

Triton::Data::Image::Image(size_t assetID, reference<PType::Bitmap> object)
	: Asset(assetID), Bitmap(object)
{
}