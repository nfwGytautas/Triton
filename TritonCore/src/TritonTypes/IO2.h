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
			 * Textures used for the material
			 * Since 00_00_00
			 */
			std::vector<std::string> Textures;

			/**
			 * Shader name used for the material
			 * Since 00_00_00
			 */
			std::string Shader;
		};

		/**
		 * Font data structure used for serializing and deserializing fonts
		 */
		struct FontData
		{
			/**
			 * Char metrics data structure used for specifying individual character attributes
			 */
			struct CharMetrics
			{
				/// Start coordinates of the character in the texture
				Vector2 Start;
				/// End coordinates of the character in the texture
				Vector2 End;
				/// Offset from the start of the image
				Vector2 Offset;
				/// Offset to next glyph
				int Advance;
			};

			/**
			 * Buffer of the font data
			 * Since 00_00_00
			 */
			std::vector<unsigned char> Buffer;

			/**
			 * Width of the font texture
			 * Since 00_00_00
			 */
			size_t Width;

			/**
			 * Height of the font texture
			 * Since 00_00_00
			 */
			size_t Height;

			/**
			 * Pixel width of a single character
			 * Since 00_00_00
			 */
			size_t PWidth;

			/**
			 * Pixel height of a single character
			 * Since 00_00_00
			 */
			size_t PHeight;

			/**
			 * The metrics for each character
			 * Since 00_00_00
			 */
			std::unordered_map<char, CharMetrics> Metrics;
		};
	}
}