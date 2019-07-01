#pragma once

//========================================================
//"http://www.glfw.org/docs/latest/group__keys.html"
//========================================================

namespace Triton
{
	enum class MouseKey : size_t
	{
		BUTTON_1 = 0,
		BUTTON_2 = 1,
		BUTTON_3 = 2,
		BUTTON_4 = 3,
		BUTTON_5 = 4,
		BUTTON_6 = 5,
		BUTTON_7 = 6,
		BUTTON_8 = 7,

		BUTTON_LAST = BUTTON_8,
		BUTTON_LEFT = BUTTON_1,
		BUTTON_RIGHT = BUTTON_2,
		BUTTON_MIDDLE = BUTTON_3
	};

	// Struct that represents the current state of the mouse
	struct Mouse
	{
		// Current scroll wheel X coordinate
		double ScrollX;
		// Current scroll wheel Y coordinate
		double ScrollY;

		// Scroll wheel X coordinate delta from last UPDATE
		double XScrollDelta;
		// Scroll wheel Y coordinate delta from last UPDATE
		double YScrollDelta;

		// Current mouse X position
		double X;
		// Current mouse Y position
		double Y;

		// Mouse position X coordinate delta from last UPDATE
		double XPosDelta;
		// Mouse position Y coordinate delta from last UPDATE
		double YPosDelta;

		// Mouse key array that specified if a key is either down or up
		// to check specific one use MouseKey enum in Triton namespace
		bool Keys[12];
	};
}