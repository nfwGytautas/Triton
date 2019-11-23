#pragma once

#define TR_NR_MAX_POINT_LIGHTS 16
#define TR_NR_MAX_DIR_LIGHTS 1
#define TR_NR_MAX_SPOT_LIGHTS 4

#define TR_NR_MAX_LIGHTS TR_NR_MAX_POINT_LIGHTS + TR_NR_MAX_DIR_LIGHTS + TR_NR_MAX_SPOT_LIGHTS

#define TR_MOUSE_KEY_COUNT 12
#define TR_KEYBOARD_KEY_COUNT 256

namespace Triton
{
	namespace Limits
	{
		// Size of the data vector in floats for 2D text
		// 3 For vertex position
		// 2 For UV
		constexpr unsigned int VAO_2D_TEXT = 3 + 2;

		// Size of the data vector in floats for non TBN models
		// 3 For vertex position
		// 2 For UV
		// 3 For normal
		constexpr unsigned int VAO_NO_TBN = 3 + 2 + 3;

		// Size of the data vector in floats with TBN information
		// 3 For vertex position
		// 2 For UV
		// 3 For normal
		// 3 for tangent
		// 3 for by-normal
		constexpr unsigned int VAO_TBN = 3 + 2 + 3 + 3 + 3;
	}
}