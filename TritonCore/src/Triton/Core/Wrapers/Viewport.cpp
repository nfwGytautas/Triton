#include "TRpch.h"
#include "Viewport.h"

Triton::Data::Viewport::Viewport(size_t assetID, reference<PType::Framebuffer> object)
	: Asset(assetID), Framebuffer(object)
{
}
