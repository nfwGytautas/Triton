#pragma once

#include <vector>

#include "TritonPlatform2/CrossTypes/Core/State/Keyboard.h"
#include "TritonPlatform2/CrossTypes/Core/State/Mouse.h"
namespace Triton
{
	namespace Graphics
	{
		/**
		 * The window is similar to the context in that each API has a specific way to create a window
		 * the Window is created using the context
		 */
		class Window
		{
		public:
			Window(IO::Keyboard* kBoard, IO::Mouse* mouse);
			virtual ~Window() { }

			/**
			 * Create a new window and initialize it
			 *
			 * @param startingWidth The starting width of the window
			 * @param startingHeight The starting height of the window
			 * @return Did the window succeeded in creating and initializing
			 */
			virtual bool initNew(float startingWidth, float startingHeight) = 0;

			/**
			 * Create a new window from an already existing native window
			 *
			 * @param params The parameters needed to create the window (Windows: HWND)
			 * @return Did the window succeeded in creating and initializing
			 */
			virtual bool initFromCreated(std::vector<void*> params) = 0;

			/**
			 * Update the window getting the inputs from the window
			 */
			virtual void update() = 0;

			/**
			 * Destroys the window
			 */
			virtual void destroy() = 0;

			/**
			 * Set the mouse cursor position
			 *
			 * @param x new X coordinate of the cursor
			 * @param y new Y coordinate of the cursor
			 */
			virtual void setCursorPos(double x, double y) = 0;

			/**
			 * Check if the window has been closed
			 * @return Window closed or not
			 */
			bool isClosed();

			/**
			 * Check if the window is currently full screen
			 * @return Window full screen status
			 */
			bool isFullscreen();

			/**
			 * Hide or show cursor
			 *
			 * @param value To hide cursor false to make cursor visible true
			 */
			virtual void showCursor(bool value) = 0;

			/**
			 * Get the size of the window
			 *
			 * @return tuple of the size first is width second is height
			 */
			virtual std::tuple<int, int> size() = 0;

			/**
			 * Sets the window to be the current one
			 */
			virtual void makeCurrent() = 0;

			/**
			 * Check if the window is currently focused
			 *
			 * @return True if focused false otherwise
			 */
			bool isFocused() const;

			/**
			 * Set the callback for drag drop files
			 *
			 * @param callback The function to be called on drag and drop operation
			 */
			void setDropCallback(std::function<void(std::vector<std::string>)> callback);

			/**
			 * Convenience function for getting the keyboard state of the context from a window
			 *
			 * @return keyboard state associated with current context
			 */
			IO::Keyboard& keyboard() const;

			/**
			 * Convenience function for getting the mouse state of the context from a window
			 *
			 * @return mouse state associated with current context
			 */
			IO::Mouse& mouse() const;
		protected:
			/// Variable to keep track if the window is initialized
			bool m_initialized = false;

			/// Variable that keeps track if the window is full screen or not
			bool m_fullscreen = false;

			/// Variable that is used to keep track if the window was created from already existing native one or not
			bool m_selfCreated = false;

			/// Variable that is used to keep track if the window was closed or not
			bool m_closed = false;

			/// Variable that is used to keep track if the window is currently focused or not
			bool m_focused = true;

			/// Variable that is used to keep track of the assigned callback for drag drop operations
			std::function<void(std::vector<std::string>)> m_dragDropCallback;

			/// Variable used to track the keyboard state, all windows share a single keyboard state
			IO::Keyboard* m_keyboardState;

			/// Variable used to track the mouse state, all windows share a single mouse state
			IO::Mouse* m_mouseState;
		};
	}
}