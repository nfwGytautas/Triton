#include "TRpch.h"
#include "Image.h"

void Triton::Data::Image::enable()
{
	object()->enable();
}

Triton::Data::Image::Image(reference<PType::Bitmap> obj)
	: PlatformWraper(obj)
{
}