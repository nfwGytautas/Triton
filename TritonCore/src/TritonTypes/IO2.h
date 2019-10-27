#pragma once

// This is an extension of the standard IO.h file that has extra IO data
// structures and definitions. They are separated into two files because
// IO.h is also used by TritonGraphics and extra definitions are of no use
// to it.

#include "IO.h"
#include "Triton2/Scene/ECS.h"

namespace Triton
{
	namespace IO
	{
		/**
		 * Material data structure used for serializing and deserializing materials
		 */
		struct MaterialData
		{
			/**
			 * Main texture name used for the material
			 * Since 00_00_00
			 */
			std::string MainTexture;

			/**
			 * Shader name used for the material
			 * Since 00_00_00
			 */
			std::string Shader;
		};
	}
}