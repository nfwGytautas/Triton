#pragma once

#include "TritonPlatform2/mathematical.h"

namespace Triton
{
	namespace Graphics
	{
		/**
		 * The buffer containing all the necessary matrices
		 */
		struct MatrixBuffer
		{
			Matrix44 Transformation;
		};

		/**
		 * The buffer containing all the vertex input information
		 */
		struct VertexInput
		{
			Vector3 Position;
			Vector2 UV;
			Vector3 Normal;
		};
	}
}