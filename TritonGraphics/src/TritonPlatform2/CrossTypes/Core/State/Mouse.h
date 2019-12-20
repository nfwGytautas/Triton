#pragma once

//========================================================
//"http://www.glfw.org/docs/latest/group__keys.html"
//========================================================

#include "Triton2//Limits.h"

#include <tuple>
#include <functional>

namespace Triton
{
	namespace IO
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

		/**
		 * Struct that represents the current state of the mouse
		 */
		class Mouse
		{
		public:
			/**
			 * Poll for the scroll of the mouse
			 *
			 * @return tuple of x and y values
			 */
			std::tuple<double, double> scroll() const;

			/**
			 * Poll for the delta of the mouse scroll
			 *
			 * @return tuple of x and y deltas
			 */
			std::tuple<double, double> deltaScroll() const;

			/**
			 * Poll for the position of the mouse
			 *
			 * @return tuple of x and y values
			 */
			std::tuple<double, double> position() const;

			/**
			 * Poll for the delta of the mouse position
			 *
			 * @return tuple of x and y  deltas
			 */
			std::tuple<double, double> deltaPosition() const;

			/**
			 * Poll for the state of the key
			 *
			 * @param key The key you want to poll for
			 * @return Is key down
			 */
			bool isKeyDown(MouseKey key) const;

			/**
			 * Add delta to scroll x value
			 *
			 * @param delta The delta to add
			 */
			void addXScrollDelta(double delta);

			/**
			 * Add delta to scroll y value
			 *
			 * @param delta The delta to add
			 */
			void addYScrollDelta(double delta);

			/**
			 * Set position x value
			 *
			 * @param newVal New x value
			 */
			void setPosX(double newVal);

			/**
			 * Set position y value
			 *
			 * @param newVal New y value
			 */
			void setPosY(double newVal);

			/**
			 * Add delta to position x and y value
			 *
			 * @param deltaX The delta to add to X
			 * @param deltaY The delta to add to Y
			 */
			void addPosDelta(double xDelta, double yDelta);

			/**
			 * Sets the state for a key
			 *
			 * @param key The key to set
			 * @prama state The new state for the key
			 */
			void setKey(MouseKey key, bool state);

			/**
			 * Set the callback for mouse down
			 *
			 * @param callback The function to be called on mouse down
			 */
			void mouseDownCallback(std::function<void(MouseKey)> callback);

			/**
			 * Set the callback for mouse up
			 *
			 * @param callback The function to be called on mouse up
			 */
			void mouseUpCallback(std::function<void(MouseKey)> callback);

			/**
			 * Set the callback for mouse move
			 *
			 * @param callback The function to be called on mouse move
			 */
			void mouseMoveCallback(std::function<void(double, double)> callback);
		private:
			/// Current scroll wheel X coordinate
			double m_scrollX;
			/// Current scroll wheel Y coordinate
			double m_scrollY;

			/// Scroll wheel X coordinate delta from last UPDATE
			double m_xScrollDelta;
			/// Scroll wheel Y coordinate delta from last UPDATE
			double m_yScrollDelta;

			/// Current mouse X position
			double m_x;
			/// Current mouse Y position
			double m_y;

			/// Mouse position X coordinate delta from last UPDATE
			double m_xPosDelta;
			/// Mouse position Y coordinate delta from last UPDATE
			double m_yPosDelta;

			/// Mouse key array that specified if a key is either down or up
			/// to check specific one use MouseKey enum in Triton namespace
			bool m_keys[TR_MOUSE_KEY_COUNT];

			/// Function to be called on mouse down input
			std::function<void(MouseKey)> m_mouseDownCallback;

			/// Function to be called on mouse up input
			std::function<void(MouseKey)> m_mouseUpCallback;

			/// Function to be called on mouse move input
			std::function<void(double, double)> m_mouseMoveCallback;
		};
	}
}