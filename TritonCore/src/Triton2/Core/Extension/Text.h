#pragma once

#include "TritonTypes/IO.h"
#include "TritonTypes/Asset.h"

namespace Triton
{
	namespace Extension
	{
		/**
		 * Create a vertex vector for the specified text
		 *
		 * @param font Triton reference to a FontAsset
		 * @param text The text to be create vertices for
		 * @param x Starting x coordinate
		 * @param y Starting y coordinate
		 * @return Vector of vertices
		 */
		IO::Mesh buildVertexArray(reference<FontAsset> font, const std::string& text, float x, float y);
	}
}