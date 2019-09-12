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
			Matrix44 Projection;
			Matrix44 View;
			Matrix44 Model;
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