#pragma once
#include <bitset>

namespace Triton
{
	namespace Flags
	{
		enum ShaderFlags
		{
			/// Flag that specifies that the shader uses settings buffers
			sFlag_Settings,

			/// Flag that specifies that the shader uses matrices buffer
			sFlag_Matrices,

			/// Flag that specifies that the shader uses lighting buffers
			sFlag_Lighting,

			/// Flag that specifies that the shader uses camera buffers
			sFlag_Camera,

			/// Flag that specifies that the shader has input for by-normals and tangents
			sFlag_TBN,

			/// Flag that specifies that the shader doesn't require normals. Ignored if TBN is set
			sFlag_NoNormals,

			/// Don't use this is used for creating a bitset of correct size
			sFlag_Count
		};

		using ShaderFlagset = std::bitset<ShaderFlags::sFlag_Count>;
	}
}